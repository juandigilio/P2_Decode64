#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

vector<char> LoadBinary(const string& filePath);
string Decode64(const string& in);


int main()
{
    vector<char> data = vector<char>();

    try
    {
        data = LoadBinary("secret_message.txt");

        for (auto character : data)
        {
            cout << character;
        }

    }
    catch (const exception& e)
    {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}


string Decode64(const string& in)
{
    string out;

    vector<int> T(256, -1);

    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0, valb = -8;

    for (unsigned char c : in) 
    {
        if (T[c] == -1)
        {
            break;
        }

        val = (val << 6) + T[c];
        valb += 6;
        
        if (valb >= 0) 
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

vector<char> LoadBinary(const string& filePath) 
{
    vector<char> output;

    ifstream file(filePath);

    if (!file) 
    {
        throw runtime_error("Unable to open file " + filePath);
    }

    stringstream buffer;
    buffer << file.rdbuf();

    string base64Content = buffer.str();
    file.close();

    string binaryContent = Decode64(base64Content);

    istringstream binaryStream(binaryContent, ios::binary);

    while (binaryStream) 
    {
        int value;

        binaryStream.read(reinterpret_cast<char*>(&value), sizeof(value));

        if (binaryStream) 
        {
            output.push_back(static_cast<char>(value & 0xFF));
            output.push_back(static_cast<char>((value >> 8) & 0xFF));
            output.push_back(static_cast<char>((value >> 16) & 0xFF));
            output.push_back(static_cast<char>((value >> 24) & 0xFF));
        }
    }

    return output;
}