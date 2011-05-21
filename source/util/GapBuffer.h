/* 
 * File:     GapBuffer.h
 * Project:  LinWarrior 3D
 * Home:     hackcraft.de
 *
 * Created on May 21, 2011, 4:10 PM
 */

#ifndef GAPBUFFER_H
#define	GAPBUFFER_H

/**
 * Models a Textbuffer by implementing a Gap-Buffer.
 */
class GapBuffer {
private:
    char* bufferData;
    unsigned long long bufferSize;
    unsigned long long bufferPre;
    unsigned long long bufferPost;
    //unsigned long bufferMark;
    char nl;
    char cl;
public:
    GapBuffer() {
        nl = '\n';
        cl = ' ';
        bufferSize = 1;
        bufferData = new char[bufferSize];
        bufferPre = 0;
        bufferPost = bufferSize;
        //bufferMark = 0;

        //test0();
    }
public: // Gap buffer operations

    unsigned long long getPosition() {
        return bufferPre;
    }

    // Seek begin of the current line.
    unsigned int getColumn() {
        int col = 0;
        while (bufferPre - col > 0 && bufferData[bufferPre - col] != nl) {
            col++;
        }
        return col;
    }

    int gotoLineBegin() {
        int col = 0;
        do {
            col++;
            if (!stepLeft()) break;
        } while (bufferData[bufferPost] != nl);
        if (bufferData[bufferPost] == nl) stepRight();
        return col - 1;
    }

    void gotoLineEnd() {
        do {
            if (!stepRight()) break;
        } while (bufferData[bufferPost] != nl);
    }

    bool stepLeft() {
        if (bufferPre > 0) {
            //cout << "leftwards\n";
            //cout << "a) "; printState_();
            bufferPost--;
            bufferPre--;
            bufferData[bufferPost] = bufferData[bufferPre];
            bufferData[bufferPre] = cl;
            //cout << "b) "; printState_();
            return true;
        }
        return false;
    }

    bool stepRight() {
        //printState_();
        if (bufferPost < bufferSize) {
            bufferData[bufferPre] = bufferData[bufferPost];
            bufferData[bufferPost] = cl;
            bufferPost++;
            bufferPre++;
            return true;
        }
        return false;
    }

    bool stepUp() {
        int col = gotoLineBegin();
        cout << "col: " << col << "\n";
        if (!stepLeft()) return false;
        gotoLineBegin();
        while (col--) {
            if (bufferData[bufferPost] == nl) break;
            if (!stepRight()) return false;
        }
        return true;
    }

    bool stepDown() {
        int col = gotoLineBegin();
        cout << "col: " << col << "\n";
        gotoLineEnd();
        if (!stepRight()) return false;
        while (col--) {
            if (bufferData[bufferPost] == nl) break;
            if (!stepRight()) return false;
        }
        return true;
    }

    unsigned long long gotoPosition(unsigned long long pos) {
        if (pos < bufferPre) {
            while (pos < bufferPre) {
                if (!stepLeft()) break;
            }
        } else {
            while (pos > bufferPre) {
                if (!stepRight()) break;
            }
        }
        return bufferPre;
    }

    void deleteLeft() {
        if (bufferPre > 0) {
            bufferData[bufferPre] = cl;
            bufferPre--;
        }
    }

    void deleteRight() {
        if (bufferPost < bufferSize) {
            bufferData[bufferPost] = cl;
            bufferPost++;
        }
    }

    void write(char c) {
        grow_();
        bufferData[bufferPre] = c;
        bufferPre++;
    }

    void write(const char* s) {
        char* p = (char*) s;
        while(*p != 0) {
            write(*p);
            p++;
        }
    }

    void write(std::string s) {
        write(s.c_str());
    }

    void writeOver(char c) {
        //bufferData[bufferPre] = c;
    }
    
    void printState(bool full = true) {
        std::cout << "pre: " << bufferPre << "(" << bufferData[bufferPre] << ")" << " post: " << bufferPost << " size: " << bufferSize;
        //std::cout << "pre: " << bufferPre << " (" << bufferData[bufferPre] << ") post: " << bufferPost << " (" << bufferData[bufferPost] << ") size: " << bufferSize;
        if (full) {
            std::cout << " data:\n";
            for (unsigned long long i = 0; i < bufferPre; i++) {
                std::cout << bufferData[i];
            }
            std::cout << "[";
            for (unsigned long long i = bufferPre; i < bufferPost; i++) {
                std::cout << bufferData[i];
                //std::cout << ".";
            }
            std::cout << "]";
            for (unsigned long long i = bufferPost; i < bufferSize; i++) {
                std::cout << bufferData[i];
            }
        }
        std::cout << std::endl;
    }

    void print() {
        for (unsigned long long i = 0; i < bufferPre; i++) {
            std::cout << bufferData[i];
        }
        for (unsigned long long i = bufferPost; i < bufferSize; i++) {
            std::cout << bufferData[i];
        }
        std::cout << std::endl;
    }

public:
    void test0() {
        std::cout << "Testing Text class\n";
        write("Hello_World!*");
        print();
        stepLeft();
        stepLeft();
        stepLeft();
        stepLeft();
        stepLeft();
        stepLeft();
        stepLeft();
        stepLeft();
        write("_to_the");
        stepRight();
        write("Electronic-");
        deleteLeft();
        write("_");
        deleteRight();
        write("V");
        print();

        if (1) {
            gotoLineEnd();
            //print();
            write("New Line");
            //print();
            gotoLineEnd();
            write(", same line*");
            //print();

            cout << "a) "; printState();
            gotoLineBegin();
            cout << "b) "; printState();
            stepLeft();
            cout << "c) "; printState();
            gotoLineBegin();
            //cout << "d) "; printState_();
            write("On the ");
            //cout << "e) "; printState_();
            print();

            cout << "a) "; printState();
            gotoLineEnd();
            cout << "b) "; printState();
            stepRight();
            write("This is the third line");
            print();
        }
    }

    void test1() {
        write("First Line!\n");
        write("Second Line!\n");
        write("Third Line!\n");
        write("Fourth Line!\n");
        printState(true);
        stepLeft();
        gotoLineBegin();
        write("On the ");
        printState(true);
        gotoLineEnd();
        printState(true);
        gotoLineBegin();
        printState(true);
        stepUp();
        printState(true);
        write("Now on the ");
        printState(true);
        gotoLineEnd();
        printState(true);
        stepUp();
        printState(true);

        gotoLineBegin();
        stepRight();
        stepRight();
        printState(true);

        stepUp();
        printState(true);

        stepDown();
        printState(true);

        stepDown();
        printState(true);

        stepLeft();
        stepLeft();

        stepUp();
        printState(true);

        stepUp();
        printState(true);

        stepUp();
        printState(true);

        stepDown();
        printState(true);

        stepDown();
        printState(true);

        stepDown();
        printState(true);
    }

private:
    void grow_() {
        if (bufferPre == bufferPost - 1) {
            unsigned long bufferSize_ = 2 * bufferSize;
            char* bufferData_ = new char[bufferSize_];
            memset(bufferData_, cl, bufferSize_);
            unsigned long i = 0;
            for (i = 0; i <= bufferPre; i++) {
                bufferData_[i] = bufferData[i];
            }
            unsigned long postsize = bufferSize - bufferPost;
            unsigned long bufferPost_ = bufferSize_ - postsize;
            for (i = 0; i < postsize; i++) {
                bufferData_[bufferPost_ + i] = bufferData[bufferPost + i];
            }
            delete bufferData;
            bufferData = bufferData_;
            bufferSize = bufferSize_;
            bufferData = bufferData_;
            bufferPost = bufferPost_;
        }
    }
};

#endif	/* TEXT_H */
