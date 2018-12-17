/**********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   In this program we maintain a File Allocation Table.
*            We will simulate "FAT-12" on a (very small) disk.
*            Each block will be 512 bytes in size. With FAT-12,
*            we have 4096 blocks, so the disk is only 2 MB in size.
*            Starting with an empty root directory, we will carry
*            out a series of transactions. From time to time we will
*            print the contents of the directory and (part) of the FAT.
***********************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <list>
#include <math.h>
#include <cstdio>
#include <iomanip>

using namespace std;

#define HOWOFTEN	6;

//A class to save all the files and their respective blocks
class Entry
{
  public:
  int size;
  string name;
  vector<int> blocks;
};

//Initialzie the functions
void newFile(string line);
void deleteFile(string line);
void copyFile(string line);
void renameFile(string line);
void modifyFile(string line);
void checkInsertDirectory();
void modifyDirectory(string line);
void newDirectory(string line);
void print();

//Directory Vector to hold the blocks of memory in order
vector<int> directory(4096);
//Vector that holds the files of Entry object type
vector<Entry> files;

list<int> lastBlock;

bool transactionCheck = false;

int blockSize = 512;

int main(int argc, char *argv[])
{
  //Initialize all directories to 0 to represent emptiness
  for(size_t i=0; i < directory.size(); i++)
    directory[i] = 0;

  lastBlock.push_back(-1);
  //Variables to handle the data file
  ifstream file;
  file.open("data7.txt");
  string line;

  //counters
  size_t t = 0;

  //first two elements
  newFile("N . 512");
  newFile("N .. 0");

  cout << "Beginning of the FAT simulation" << endl << endl;

  size_t z = HOWOFTEN;
  //go through file and read line by line
  while(!file.eof())
  {

    if(t%z == 0)
    {
      cout << endl;
      print();//print every 5 times
    }

    //get line and pass it accoringly to its method
    getline(file,line);
    if(line.at(0) == 'R')
    { //process to rename the file name
      cout << "Transaction:  Rename a file" << endl;
      renameFile(line);
      istringstream iss(line);
      char a;
      string b, c;
      iss >> a >> b >> c;
      if(transactionCheck == true)
      { //print out a success message it the transaction is successful
        cout << "Successfully changed the file name " << b << " to " << c;
        transactionCheck = false;
      }
    }
    else if(line.at(0) == 'M')
    { //process to modify the file
      cout << "Transaction:  Modify a file" << endl;
      modifyFile(line);
      if(transactionCheck == true)
      { //print out a success message it the transaction is successful
        cout << "Successfully modified a file, " << files.back().name << endl;
        transactionCheck = false;
      }
    }
    else if(line.at(0) == 'N')
    { //process to create a file
      cout << "Transaction:  Add a new file" << endl;
      newFile(line);
      if(transactionCheck == true)
      { //print out a success message it the transaction is successful
        cout << "Successfully added a new file, " << files.back().name << ", of size " << files.back().size << endl;
        transactionCheck = false;
      }
    }
    else if(line.at(0) == 'D')
    { //process to delete the file
      cout << "Transaction:  Delete a file" << endl;
      deleteFile(line);
      istringstream iss(line);
      char a;
      string b;
      iss >> a >> b;
      if(transactionCheck == true)
      { //print out a success message it the transaction is successful
        cout << "Successfully deleted a file, " << b << endl;
        transactionCheck = false;
      }
    }
    else if(line.at(0) == 'C')
    { //process to copy the file
      cout << "Transaction:  Copy a file" << endl;
      copyFile(line);
      istringstream iss(line);
      char a;
      string b, c;
      iss >> a >> b >> c;
      if(transactionCheck == true)
      { //print out a success message it the transaction is successful
        cout << "Successfully copied an existing file, " << b << ", to a new file, " << c << endl;
        transactionCheck = false;
      }
    }
    else if(line.at(0) == '?')
      break;

    t++;
  }
  file.close();

  cout << "End of the FAT simulation" << endl << endl;
  print();//print lists after all is over

}

//function to add new file to lists
void newFile(string line)
{

  //variables to get info from line
  char trans;
  string name;
  int size;

  //boolean to check if element already exists
  bool found = false;
  //object to store as file
  Entry file;

  //get info from line
  istringstream iss(line);
  iss >> trans >> name >> size;

  //check for file existence
  for(size_t i=0; i < files.size(); i++)
  {
    if(files.at(i).name == name)
      found = true;
  }

  if(found)
    cout<<"File already exists can not create new file: "<<name<<endl;
  else
  {
    file.name = name;
    file.size = size;
    transactionCheck = true;
    //get amount of blocks to use
    int amountOfBlocks = ceil((float)size / blockSize);


    //search through directory check for 0 and change it to j for the amount of blocks we have
    int i = 1;

    while(i <= amountOfBlocks)
    {
      for(size_t j=0; j < directory.size(); j++)
      {
        //if the last block then set it up to be -1
        if(directory.at(j) == 0 && i == amountOfBlocks)
        {
          directory.at(j) = -1;
          file.blocks.push_back(j);
          i++;
          break;
        }
        //else change from 0 to j+1
        else if(directory.at(j) == 0)
        {
          directory.at(j) = j+1;
          file.blocks.push_back(j);
          i++;
          break;

        }
      }
    }

    //put object in list
    files.push_back(file);

    size_t ctr = 0;
    //loop to point the current position to point to the next position
    for(vector<int>::iterator it = file.blocks.begin(); it != file.blocks.end();)
    {
      size_t x = *it;
      it++;
      size_t y = *it;
      ctr++;

      if(file.blocks.size() == ctr)
      {
        directory.at(x) = -1;
        break;
      }
      else
      {
        directory.at(x) = y;
      }
    }
    //check to see if the size of the directory has changed.
    checkInsertDirectory();
  }

}

//method to delete file.
void deleteFile(string line){

  char trans;
  string name;

  istringstream iss(line);
  iss >> trans >> name;

  //go through the blocks of the file
  //go through directory and change the value to 0 accoring to the blocks of the file
  for(size_t i = 0; i < files.size(); i++)
  {
    if(name == files.at(i).name)
    {
      transactionCheck = true;
      for(size_t j = 0; j < files.at(i).blocks.size(); j++)
        directory.at(files.at(i).blocks.at(j)) = 0;
      files.erase(files.begin() + i );//erase file from list
    }
  }
}

//copy file method
void copyFile(string line){
  char trans;
  string name;
  string newName;
  bool found = false;
  bool copyFound = false;
  Entry file;


  istringstream iss(line);
  iss >> trans >> name >> newName;

  //save the index of the found file
  for(size_t i = 0; i < files.size(); i++){
    if(files.at(i).name == name){
      file.size = files.at(i).size;
      found = true;
      //search the list to see if the copy already exists
      for(size_t j = 0; j < files.size(); j++){
        if(files.at(j).name == newName){
          copyFound = true;
        }
      }
    }
  }

  if(!found){
    cout<<"Cannot find file to copy: "<<name<<endl;
  }
  else{
    if(copyFound)
      cout<<"Copy with the same name as the new copy found: "<<newName<<endl;
    else{
      file.name = newName;
      transactionCheck = true;
      //get the amount of blocks
      int amountOfBlocks = ceil((float)file.size / blockSize);

        //change the empty elements from 0 to j+1
        int i = 1;
        while(i <= amountOfBlocks){
          for(size_t j=1; j < directory.size(); j++){
            if(directory.at(j) == 0 && i == amountOfBlocks){
              directory.at(j) = -1;
              file.blocks.push_back(j);
              i++;
              break;
            }
            if(directory.at(j) == 0){
              directory.at(j) = j+1;
              file.blocks.push_back(j);
              i++;
              break;
            }
          }
        }
        files.push_back(file);
        size_t ctr = 0;
        for(vector<int>::iterator it = file.blocks.begin(); it != file.blocks.end();)
        {
          size_t x = *it;
          it++;
          size_t y = *it;
          ctr++;

          if(file.blocks.size() == ctr)
          {
            directory.at(x) = -1;
            break;
          }
          else
          {
            directory.at(x) = y;
          }
        }
    }
  }
}

//method to rename the file
void renameFile(string line)
{
  char trans;
  string name;
  string newName;
  bool found = false;
  bool copyFound = false;
  int index;

  istringstream iss(line);
  iss >> trans >> name >> newName;

  //find the file and set the boolean to true
  for(size_t i = 0; i < files.size(); i++)
  {
    if(files.at(i).name == name)
    {
      found = true;
      index = i;
    }
    //search if file atlready exists
    if(files.at(i).name == newName)
      copyFound = true;
  }

  if(!found)
    cout<<"No File found to be renamed, File: "<<name<<endl;
  else
  {
    if(copyFound)
      cout<<"File with name of copy already exists: "<<newName<<endl;
    else
    {
      transactionCheck = true;
      files.at(index).name = newName;
    }
  }
}

//change size of file
void modifyFile(string line){

  char trans;
  string name;
  bool found = false;

  istringstream iss(line);
  iss >> trans >> name;

  //delete file
  for(size_t i = 0; i < files.size(); i++)
  {
    if(name == files.at(i).name)
    {
      transactionCheck = true;
      Entry file;
      file.name = files.at(i).name;
      file.size = files.at(i).size;
      for(size_t k = 0; k < files.at(i).blocks.size(); k++)
      {
        file.blocks.push_back(files.at(i).blocks.at(k));
      }
      files.erase(files.begin() + i);
      newFile(line);
      for(size_t j = 0; j < file.blocks.size(); j++)
      {
        directory.at(file.blocks.at(j)) = 0;
        lastBlock.push_back(j+1);
      }
      found = true;
      break;
    }
  }
  //reinsert new file.
  if(!found)
    cout<<"File to modify changed"<<endl;

}
void checkInsertDirectory()
{ //check the size of the directory
  if(files.size() > 12 && files.size() <= 24)
  {
    if(files.at(0).size == 1024)
    {
      return;
    }
    else
    {
      cout << "Change Size " << files.at(0).size << " to 1024" << endl;
      modifyDirectory("M . 1024");
    }

  }
  else if(files.size() > 24)
  {
    if(files.at(0).size == 1536)
    {
      return;
    }
    else
    {
      cout << "Change Size "  << files.at(0).size << " to 1536"<< endl;
      modifyDirectory("M . 1536");
    }
  }
}
void modifyDirectory(string line)
{
  char trans;
  string name;

  istringstream iss(line);
  iss >> trans >> name;

  //delete file
  for(size_t i = 0; i < files.size(); i++){
    if(name == files.at(i).name){
      Entry file;
      file.name = files.at(i).name;
      file.size = files.at(i).size;
      for(size_t k = 0; k < files.at(i).blocks.size(); k++)
      {
        file.blocks.push_back(files.at(i).blocks.at(k));
      }
      files.erase(files.begin() + i);
      for(size_t j = 0; j < file.blocks.size(); j++)
      {
        directory.at(file.blocks.at(j)) = 0;
        lastBlock.push_back(j+1);
      }
      newDirectory(line);

      break;
    }
  }
}
void newDirectory(string line)
{
  //variables to get info from line
  char trans;
  string name;
  int size;

  //boolean to check if element already exists
  bool found = false;
  //object to store as file
  Entry file;

  //get info from line
  istringstream iss(line);
  iss >> trans >> name >> size;

  //check for file existence
  for(size_t i=0; i < files.size(); i++)
  {
    if(files.at(i).name == name)
      found = true;
  }

  if(found)
    cout<<"File already exists can not create new file: "<<name<<endl;
  else
  {
    file.name = name;
    file.size = size;

    //get amount of blocks to use
    size_t amountOfBlocks = ceil((float)size / blockSize);


    //search through directory check for 0 and change it to j for the amount of blocks we have
    size_t i = 1;

    while(i <= amountOfBlocks)
    {
      for(size_t j=0; j < directory.size(); j++)
      {

        //if the last block then set it up to be -1
        if(directory.at(j) == 0 && i == amountOfBlocks)
        {
          directory.at(j) = -1;
          file.blocks.push_back(j);
          i++;
          break;
        }
        //else change from 0 to j+1
        else if(directory.at(j) == 0)
        {
          directory.at(j) = j+1;
          file.blocks.push_back(j);
          i++;
          break;

        }
      }
    }

    //put object in list
    files.insert(files.begin(),file);

    size_t ctr = 0;
    for(vector<int>::iterator it = file.blocks.begin(); it != file.blocks.end();)
    {
      size_t x = *it;
      it++;
      size_t y = *it;
      ctr++;

      if(file.blocks.size() == ctr)
      {
        directory.at(x) = -1;
        break;
      }
      else
      {
        directory.at(x) = y;
      }
    }
    checkInsertDirectory();
  }
}
//print both lists
void print(){

  int j=0;
  int totalSize = 0;

  cout << "Directory Listing" << endl;
  for(size_t i =0; i < files.size(); i++)
  {
    cout << "File Name: " << left << setw(25) << files.at(i).name << " File Size:\t" << files.at(i).size << endl;
    totalSize += files.at(i).size;
    cout << "Cluster(s) in use:\t";
    if(files.at(i).size == 0)
      cout << "(none)";
    else
    {
      for(size_t j=0; j < files.at(i).blocks.size(); j++)
      {
        cout << setw(6) << files.at(i).blocks.at(j);
      }
    }
    cout << endl;
  }
  cout << "Files:  " << files.size() << "\t" << "Total Size:  " << totalSize << endl << endl;
  cout << "Contents of the File Allocation Table" << endl;
  for(int i =0; i < 240; i++)
  {
    if(j % 12 == 0){
      printf("#%03d-%03d", i , i+11);
    }
    cout << right << setw(7) << directory.at(i);
    j++;
    if(j % 12 == 0){
      cout << endl;
      j = 0;
    }
  }
  cout << endl;
}
