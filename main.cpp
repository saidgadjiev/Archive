//
//  main.cpp
//  MyArchiever
//
//  Created by Саид on 06.08.15.
//  Copyright (c) 2015 Саид. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <limits>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem/operations.hpp>
using namespace std;
using namespace boost::filesystem;
using boost::regex;

template <class T>
void destroy(T *p) {
    p->~T();
}

template <class FwdIter>
void destroy(FwdIter *first, FwdIter *last) {
    while (first != last) {
        destroy(&(*first));
        ++first;
    }
}

template <class T>
class CHeapImpl {
public:
    CHeapImpl(size_t size = 0);
    ~CHeapImpl();
    void swap(CHeapImpl &tmp) throw();
    
    T **buffer;
    size_t realSize;
    size_t bufferSize;
private:
    CHeapImpl(const CHeapImpl &tmp);
    CHeapImpl &operator = (const CHeapImpl &tmp);
};

template <class T>
CHeapImpl<T>::CHeapImpl(size_t size):
    buffer(static_cast<T**>(size == 0
                            ? nullptr
                            : operator new(sizeof(T) * size))),
    realSize(0),
    bufferSize(size)
{}

template <class T>
CHeapImpl<T>::~CHeapImpl() {
    destroy(buffer, buffer + realSize);
    operator delete(buffer);
}

template <class T>
void CHeapImpl<T>::swap(CHeapImpl &tmp) throw() {
    ::swap(buffer, tmp.buffer);
    ::swap(realSize, tmp.realSize);
    ::swap(bufferSize, tmp.bufferSize);
}

template <class T>
class CHeap {
public:
    CHeap(size_t size = 0);
    void add(T *item);
    void deleteExtractElement();
    T *extractMin();
    inline bool isEmpty() const;
    inline bool isFull() const;
    void printHeap() const;
    inline size_t getSize() const;
private:
    CHeapImpl<T> impl;
    void grow();
    void siftUp(size_t index);
    void siftDown(size_t index);
};

template <class T>
CHeap<T>::CHeap(size_t size):
    impl(size)
{}

template <class T>
void CHeap<T>::grow() {
    CHeap temp(impl.bufferSize * 2 + 1);

    while (temp.getSize() < impl.realSize) {
        temp.add(impl.buffer[temp.getSize()]);
    }
    impl.swap(temp.impl);
}

template <class T>
void CHeap<T>::add(T *item) {
    if (isFull()) {
        grow();
    }
    impl.buffer[impl.realSize++] = item;
    siftUp(impl.realSize - 1);
}

template <class T>
T *CHeap<T>::extractMin() {
    if (isEmpty()) {
        throw "Heap empty";
    } else {
        return impl.buffer[0];
    }
}

template <class T>
void CHeap<T>::deleteExtractElement() {
    if (isEmpty()) {
        throw "extract from empty heap";
    } else {
        impl.buffer[0] = impl.buffer[impl.realSize - 1];
        impl.realSize--;
        destroy(impl.buffer + impl.realSize);
        siftDown(0);
    }
}

template <class T>
void CHeap<T>::siftUp(size_t index) {
    while( index > 0 ) {
        size_t parent = (index - 1) / 2;
        
        if (*(impl.buffer[parent]) <= *(impl.buffer[index])) {
            return;
        }
        swap(*(impl.buffer[index]), *(impl.buffer[parent]));
        index = parent;
    }
}

template <class T>
void CHeap<T>::siftDown(size_t index)
{
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t smallest = index;
    
    if (left < impl.realSize && *(impl.buffer[left]) < *(impl.buffer[index])) {
        smallest = left;
    }
    if (right < impl.realSize && *(impl.buffer[right]) < *(impl.buffer[smallest])) {
        smallest= right;
    }
    if(smallest != index) {
        swap(impl.buffer[index], impl.buffer[smallest] );
        siftDown(smallest);
    }
}

template <class T>
void CHeap<T>::printHeap() const {
    for (int i = 0; i < impl.realSize; i++) {
        cout << *(impl.buffer[i]) << endl;
    }
}

template <class T>
size_t CHeap<T>::getSize() const {
    return impl.realSize;
}

template <class T>
bool CHeap<T>::isEmpty() const {
    return impl.realSize == 0;
}

template <class T>
bool CHeap<T>::isFull() const {
    return impl.realSize == impl.bufferSize;
}

class CTree {
public:
    CTree(unsigned long int weight = 0, unsigned char ch = '\0');
    ~CTree();
    void destroyTree(CTree *node) const;
    inline void setWeight(unsigned long int weight);
    inline unsigned long int getWeight() const;
    inline void setChar(unsigned char ch);
    inline unsigned char getChar() const;
    inline void setLeft(CTree *node);
    inline CTree *getLeft() const;
    inline void setRight(CTree *node);
    inline CTree *getRight() const;
    void printNode() const;
    bool findChar(CTree *node, const string bitCode, unsigned char &ch) const;
    void buildHuffmanCode(CTree *node, unsigned char ch, string bitCode, string &result) const;
    void printTree(CTree *node) const;
    bool operator < (const CTree &node) const;
    bool operator <= (const CTree &node) const;
    bool operator > (const CTree &node) const;
    bool operator >= (const CTree &node) const;
    friend ostream &operator << (ostream &os, const CTree &node);
private:
    unsigned long int weight;
    unsigned char ch;
    CTree *left, *right;
};

CTree::CTree(unsigned long int weight, unsigned char ch):
    weight(weight),
    ch(ch),
    left(nullptr),
    right(nullptr)
{}

CTree::~CTree()
{
    if (left) {
        destroyTree(left);
    } else {
        destroyTree(right);
    }
}

void CTree::destroyTree(CTree *node) const
{
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

unsigned long int CTree::getWeight() const
{
    return weight;
}

unsigned char CTree::getChar() const
{
    return ch;
}

void CTree::setWeight(unsigned long int freq)
{
    weight = freq;
}

void CTree::setLeft(CTree *node)
{
    left = node;
}

void CTree::setRight(CTree *node)
{
    right = node;
}

CTree *CTree::getLeft() const
{
    return left;
}

CTree *CTree::getRight() const
{
    return right;
}

void CTree::printNode() const
{
    cout << weight << " " << ch;
}

void CTree::printTree(CTree *node) const
{
    if (node) {
        cout << node->weight << endl;
        printTree(node->left);
        printTree(node->right);
    }
}

void countFrequence(ifstream &file, unsigned long int *weight)
{
    unsigned char ch = '\0';
    char tmp = '\0';
    
    while (file.get(tmp)) {
        ch = tmp;
        weight[ch]++;
    }
    file.clear();
    file.seekg(0);
}

ostream &operator << (ostream &os, const CTree &node)
{
    return os << node.weight << " " << node.ch;;
}

bool CTree::operator < (const CTree &node) const
{
    return weight < node.weight;
}

bool CTree::operator <= (const CTree &node) const
{
    return weight <= node.weight;
}

bool CTree::operator > (const CTree &node) const
{
    return weight > node.weight;
}

bool CTree::operator >= (const CTree &node) const
{
    return weight >= node.weight;
}

void CTree::buildHuffmanCode(CTree *node, unsigned char ch, string bitCode, string &result) const
{
    if (node) {
        if (!node->left && !node->right && node->ch == ch) {
            result = bitCode;
        } else {
            buildHuffmanCode(node->left, ch, bitCode + "0", result);
            buildHuffmanCode(node->right, ch, bitCode + "1", result);
        }
    }
}

bool CTree::findChar(CTree *node, const string bitCode, unsigned char &ch) const
{
    for(size_t i = 0; i < bitCode.size(); i++) {
        if(bitCode[i] == '0') {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    bool result = false;
    
    if(!node->left && !node->right) {
        result = true;
        ch = node->ch;
    }
    
    return result;
}

void writeBitCodeInFile(unsigned char bit, ostream &file)
{
    static int bitPos = 0;
    static unsigned char ch = '\0';
    
    if (bit < 2) {
        if (bit == 1) {
            ch |= (1 << (CHAR_BIT - 1 - bitPos));
        } else {
            ch &= static_cast<unsigned char>(UCHAR_MAX - (1 << (CHAR_BIT - 1 - bitPos)));
        }
        ++bitPos;
        bitPos %= CHAR_BIT;
        if (bitPos == 0) {
            file.put(ch);
            ch = '\0';
        }
    } else {
        file.put(ch);
    }
}

CTree *buildTree(unsigned long int *weight)
{
    shared_ptr<CHeap<CTree>> heap(new CHeap<CTree>());
    CTree *node;
    
    for (unsigned short i = 0; i < UCHAR_MAX + 1; i++) {
        if (weight[i] > 0) {
            node = new CTree(weight[i], static_cast<unsigned char>(i));
            heap->add(node);
        }
    }
    CTree *node1;
    CTree *node2;
    
    do {
        node1 = heap->extractMin();
        heap->deleteExtractElement();
        if (!heap->isEmpty()) {
            node2 = heap->extractMin();
            heap->deleteExtractElement();
            node = new CTree();
            node->setWeight(node1->getWeight() + node2->getWeight());
            node->setLeft(node1);
            node->setRight(node2);
            heap->add(node);
        }
    } while (!heap->isEmpty());
    
    return node1;
}

unsigned char readCodeFromFile(ifstream &infile)
{
    static int bitPos = 0;
    static unsigned char ch = infile.get();
    unsigned char bitCode;
    
    bitCode = (ch >> (CHAR_BIT - 1 - bitPos)) % 2;
    ++bitPos;
    bitPos %= CHAR_BIT;
    if (bitPos == 0) {
        if (!infile.eof())
            ch = infile.get();
        else
            bitCode = 2;
    }
    
    return bitCode;
}

void encoder(const string ifile, const string ofile)
{
    ifstream infile(ifile.c_str(), ios::in|ios::binary);
    
    if (!infile.is_open()) {
        cerr << "Error can't open file: " << ifile << endl;
        exit(-1);
    }
    ofstream outfile(ofile.c_str(), ios::out|ios::binary|ios::trunc);
    unsigned long int weight[UCHAR_MAX + 1] = {0};
    
    countFrequence(infile, weight);
    for (unsigned short i = 0; i < UCHAR_MAX + 1; i++) {
        outfile.put(static_cast<unsigned char>(weight[i] >> 24));
        outfile.put(static_cast<unsigned char>((weight[i] >> 16) % (UCHAR_MAX + 1)));
        outfile.put(static_cast<unsigned char>((weight[i] >> 8) % (UCHAR_MAX + 1)));
        outfile.put(static_cast<unsigned char>(weight[i] % (UCHAR_MAX + 1)));
    }
    string bitStrings[UCHAR_MAX + 1];
    shared_ptr<CTree> root(buildTree(weight));
    
    for (unsigned short i = 0; i < UCHAR_MAX + 1; i++) {
        if (weight[i] > 0) {
            bitStrings[i] = "";
            i = static_cast<unsigned char>(i);
            root->buildHuffmanCode(root.get(), i, "", bitStrings[i]);
        }
    }
    unsigned char ch = '\0';
    char tmp = '\0';
    unsigned char bit = '\0';
    
    while (infile.get(tmp)) {
        ch = tmp;
        if (weight[ch] > 0) {
            for (size_t i = 0; i < bitStrings[ch].size(); i++) {
                if (bitStrings[ch].at(i) == '0') {
                    bit = 0;
                } else {
                    bit = 1;
                }
                writeBitCodeInFile(bit, outfile);
            }
        }
    }
    bit = 2;
    writeBitCodeInFile(bit, outfile);
    infile.close();
    outfile.close();
}

void decoder(const string ifile, const string ofile)
{
    ifstream infile(ifile.c_str(), ios::in|ios::binary);
    
    if (!infile.is_open()) {
        cerr << "Error can't open file: " << ifile << endl;
        exit(-1);
    }
    ofstream outfile(ofile.c_str(), ios::out|ios::binary|ios::trunc);
    unsigned long int weight[UCHAR_MAX + 1] = {0};
    unsigned char ch = '\0';
    char tmp = '\0';
    
    for (unsigned short i = 0; i < UCHAR_MAX + 1; i++) {
        for (int j = 3; j >= 0; j--) {
            infile.get(tmp);
            ch = tmp;
            weight[i] += ch * (1 << (CHAR_BIT * j));
        }
    }
    shared_ptr<CTree> root(buildTree(weight));
    unsigned long int totalWeight = root->getWeight();
    string bitStr = "";
    unsigned char bitChar = '\0';
    
    while (totalWeight > 0) {
        bitStr = "";
        do {
            ch = readCodeFromFile(infile);
            if (ch == 0) {
                bitStr += '0';
            } else {
                bitStr += '1';
            }
        } while (!root->findChar(root.get(), bitStr, bitChar));
        outfile.put(static_cast<char>(bitChar));
        totalWeight--;
    }
    infile.close();
    outfile.close();
}

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        cerr << "Error arguments! Please enter <name_directory> key <-e> for encode <-d> for decode!" << endl;
        return -1;
    }
    path in(argv[1]);
    string dir(argv[1]);
    boost::regex reg("\\.((\\w)|(\\d))*");
    vector<path> filePath;
    
    if (is_directory(in)) {
        for (recursive_directory_iterator rdib(in), rdie; rdib != rdie; rdib++) {
            string filename = rdib->path().filename().string();
            
            if (!boost::regex_match(filename, reg, boost::match_partial)) {
                filePath.push_back(rdib->path());
            }
        }
        if (!strcmp("-e", argv[2])) {
            string prefix(".huff");
            if (dir.find(prefix) != string::npos) {
                cerr << "Error arguments! Please enter encode file!" << endl;
                return -1;
            }
            path outDir(dir + ".huff");
            if (exists(outDir)) {
                remove_all(outDir);
            }
            create_directory(outDir);
            for(auto path: filePath) {
                encoder(path.string(), outDir.string() + "/" + path.filename().string() + ".huff");
            }
        } else if (!strcmp("-d", argv[2])) {
            string prefix(".huff");
            size_t pos = dir.find(prefix);
            if (pos == string::npos) {
                cerr << "Error arguments! Please enter decode file .*!" << endl;
                return -1;
            }
            dir.erase(pos, prefix.size());
            path outDir(dir);
            if (exists(outDir)) {
                remove_all(outDir);
            }
            create_directory(outDir);
            for(auto path: filePath) {
                string ofile = path.filename().string();
                size_t pos = ofile.find(prefix);
                
                if (pos != string::npos)
                {
                    ofile.erase(pos, prefix.size());
                }
                decoder(path.string(), outDir.string() + "/" + ofile);
            }
        }
    } else {
        if (!strcmp("-e", argv[2])) {
            string prefix(".huff");
            if (dir.find(prefix) != string::npos) {
                cerr << "Error arguments! Please enter encode file!" << endl;
                return -1;
            }
            path outFile(dir + ".huff");
            encoder(in.filename().string(), outFile.string());
        } else if (!strcmp("-d", argv[2])) {
            string prefix(".huff");
            size_t pos = dir.find(prefix);
            if (pos == string::npos) {
                cerr << "Error arguments! Please enter decode file .*!" << endl;
                return -1;
            }
            dir.erase(pos, prefix.size());
            path outDir(dir);
            decoder(in.filename().string(), outDir.string());
        }
    }
     
    return 0;
}
