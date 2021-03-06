#include <cstring>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
using namespace std;


unsigned int key[4]={0xFDA5,0xD54E,0xFC00,0xB55A}; // encryption key

#define BLOCK_SIZE 8

int copy(string file1, string file2){
  register int filekey;
  
  FILE *fp1, *fp2;
  fp1 = fopen(file1.c_str(), "a+");
  fp2 = fopen(file2.c_str(), "a+");
  
  while ((filekey = fgetc(fp1)) != EOF)
   {
    fputc(filekey, fp2);
   }
  fclose(fp1); fclose(fp2);
  
 return 0;
}

void xtea_encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void xtea_decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void FileCrypt(string filename,bool encrypt)
{
  fstream file(filename.c_str(),ios::in | ios::out | ios::binary);

  if(!file)
    {
      cout <<"Could not open file" << endl;
      return;
    }

  unsigned size;

  file.seekg(0,ios::end);
  size=file.tellg();
  file.seekg(ios::beg);

  file.clear();

  unsigned pos;

  int n_blocks=size/BLOCK_SIZE;
  if(size%BLOCK_SIZE!=0)
      ++n_blocks;

  for(int i=0;i<n_blocks;i++)
    {
      unsigned char data[BLOCK_SIZE];
      pos=file.tellg();

      file.read((char*)data,BLOCK_SIZE); // read data block

      if(encrypt)
          xtea_encipher(32,(uint32_t*)data,key);
      else
          xtea_decipher(32,(uint32_t*)data,key);

      file.seekp(pos);
      file.write((char*)data,BLOCK_SIZE);

      memset(data,0,BLOCK_SIZE);
    }
  file.close();
}

int main( int argc, const char *argv[])
{
  if(argc<2){
    cout << "Usage: " << argv[0] << "[file] [optional 4 byte key]\n";
    return 0;
  }
  string filepath = argv[1];
  if(argc>2){
    //cout << "argc = "<<argc<<endl;
    //cout << "argv[2] = " << argv[2] << endl;
    string ukey = argv[2];
    //cout << "ukey = " << ukey << endl;
    if(ukey.length()>4 || ukey.length()<4){
      cout << "Key must be 4 bytes long\n";
      return 0;
    }
     key[0] = ukey.c_str()[0];
     key[1] = ukey.c_str()[1];
     key[2] = ukey.c_str()[2];
     key[3] = ukey.c_str()[3];
  }
  string tmppath = "/tmp/un.pac";
  copy(filepath, tmppath);
  FileCrypt(tmppath, false);
  fstream file(tmppath.c_str(),ios::in | ios::out | ios::binary);
  file.seekg(0, std::ios_base::end);
  int size = file.tellg();
  file.seekg(size-8, ios::beg);
  char data[8];
  file.read((char*)data, 8);
  file.seekg(0, ios::beg);
  string Sdata= data;
  file.close();
  if(Sdata.find("p!3") != string::npos){
        remove(filepath.c_str());
        register int filekey;
        register int filekey2;
        register int filekey3;
        FILE *fp1, *fp2;
        fp1 = fopen(tmppath.c_str(), "a+");
        fp2 = fopen(filepath.c_str(), "a+");
        
        while ((filekey = fgetc(fp1)) != EOF)
         {
          if(filekey == 'p'){
            if((filekey2 = fgetc(fp1)) == '!'){
              if((filekey3 = fgetc(fp1)) == '3'){
                break;
              }
              fputc(filekey, fp2);
              fputc(filekey2, fp2);
              fputc(filekey3, fp2);
            }
            fputc(filekey, fp2);
            fputc(filekey2, fp2);
          }
          else{
            fputc(filekey, fp2);
          }
         }
        fclose(fp1); fclose(fp2);
      remove(tmppath.c_str());
  }
  else{
    cout << "Decryption failed (Wrong passphrase?)" << endl;
    remove(tmppath.c_str());
    return -1;
  }
  remove(tmppath.c_str());
}
