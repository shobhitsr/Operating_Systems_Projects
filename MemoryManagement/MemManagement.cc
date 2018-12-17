/********************************************************************
* Progammer: SHOBHIT SRIVASTAVA
*
* Purpose:   In this program we simulate a Memory Management.
*            We load some memory from a given file and then allocate
*            it to the memory of the system. For the deallocation we
*            remove all the blocks of the process and then terminate
*            the process and free the memory.
*********************************************************************/

#include "MemManagement.h"
int main(int argc, char* argv[])
{
    //Check if 2 arguments are given or not.
    if (argc != 2)
    {
        cout << "Please specify B or F" << endl;
        exit(-1);
    }
    ifstream myfile ("data6.txt");      //Open the file
    char Algorithm = *argv[1];          //Get the character B or F

    Avail = populateAvail(Avail);       //Populate the Available list with the available memory

    //If the user chooses Best fit perform this algorithm
    if (Algorithm == 'B')
    {
        cout << "Simulation of Memory Management using the Best-Fit algorithm" << endl;
        cout << endl << "Beginning of the run" << endl;
        printLists(Avail,Inuse);        //Print the Available and the in use list at the start of the algorithm
        cout << endl;
        best_fit(myfile);       //Implement the best fit algorithm on the given file
        cout << endl << endl <<"End of the run" << endl;
        printLists(Avail,Inuse);        //Print the Available and the in use list at the end of the algorithm
    }
    //If the user chooses First fit perform this algorithm
    else if (Algorithm == 'F')
    {
        cout << "Simulation of Memory Management using the First-Fit Algorithm" << endl;
        cout << endl << "Beginning of the run" << endl;
        printLists(Avail,Inuse);    //Print the Available and the in use list at the start of the algorithm
        first_fit(myfile);      //Implement the first fit algorithm on the given file
        cout << endl << endl <<"End of the run" << endl;
        printLists(Avail,Inuse);    //Print the Available and the in use list at the end of the algorithm
    }
    else
    {
        //If the input is invalid
        cout << "Incorrect Value Inserted" << endl;
        cout << "Please specify B or F" << endl;
        exit(-1);
    }
    return 0;
}

//Best fit algorithm where the choosen memory is
//the most space efficient
void best_fit(ifstream &myfile)
{
    if(myfile.is_open())
    {
        //Declare variables
        string line;
        vector<string> v;
        string word;

        //Get each line and tokenize each word
        while(getline(myfile,line))
        {
            istringstream iss(line);
            for(word; iss >> word; )
            v.push_back(word);
        }
        //Iterate through the words
        for (vector<string>::iterator t=v.begin(); t!=v.end(); ++t)
        {
            string x = *t;
            //If load transaction is selected
            if(x == "L")
            {
                int Address, blockSize;
                string PID,BID;
                t++; PID = *t;
                t++; blockSize = stoi(*t);
                t++; BID = *t;
                cout << "Transaction:  request to load process " << PID <<", block ID " << BID << " using " << blockSize << endl;
                //Perform the load algorithm
                if (loadTransaction(PID,BID,blockSize))
                    cout << "Success in allocating a block" << endl;
                else
                    cout << "Unable to allocate a block" << endl;
                printLists(Avail,Inuse);        //Print the contents of Available and Inuse list
            }
            //If terminate process is selected
            else if(x == "T")
            {
                t++;
                cout << "Transaction:  request to terminate process " << *t << endl;
                //Perform the terminate algorithm
                if(terminateProcess(*t))
                {
                    for(list<Memory>::iterator it = Avail.begin(), prev = Avail.end(); it != Avail.end(); prev = it, ++it)
                    {
                        //Check if the memory of consicutive blocks is less than 4 MB
                        if(prev != Avail.end() && (it -> getsize() + prev -> getsize()) < 4*MB)
                        {
                            //Perform a meage of the blocks that are less than 4 MB
                            cout << "Merging two blocks at " << prev -> getStartAddress() << " and " << it -> getStartAddress() << endl;
                            it -> setsize(it -> getsize() + prev -> getsize());
                            it->setStartAddress(prev->getStartAddress());
                            Avail.erase(prev);
                            continue;
                        }
                    }
                    cout << "Success in terminating a process" << endl;
                }
                else
                cout << "Unable to comply as the indicated process could not be found." << endl;
                printLists(Avail,Inuse);        //Print the contents of Available and Inuse list
            }
            //If Allocate block is selected
            else if(x == "A")
            {
                //Assign vaiables
                int Address, blockSize;
                string PID,BID;
                t++; PID = *t;
                t++; blockSize = stoi(*t);
                t++; BID = *t;
                cout << "Transaction:  request to allocate " << blockSize <<" bytes for process " << PID << ", block ID: " << BID << endl;
                //Perform the allocate algorithm
                if (allocateTransaction(PID,BID,blockSize))
                    cout << "Success in allocating a block" << endl;
                else
                    cout << "Unable to comply as the indicated process could not be found." << endl;
                printLists(Avail,Inuse);        //Print the contents of Available and Inuse list
            }
            //If Deallocate block is selected
            else if(x == "D")
            {
                //Assign Variables
                string PID, BID;
                cout<<*t<< " ";
                t++; PID = *t; cout<<PID<< " ";
                t++; BID = *t; cout<<BID<<endl;
                //Perform the deallocate algorithm
                if(deallocateBlock(PID, BID))
                {
                    for(list<Memory>::iterator it = Avail.begin(), prev = Avail.end(); it != Avail.end(); prev = it, ++it)
                    {
                        //Check if the memory of consicutive blocks is less than 4 MB
                        if(prev != Avail.end() && (it -> getsize() + prev -> getsize()) < 4*MB)
                        {
                            //Perform a meage of the blocks that are less than 4 MB
                            cout << "Merging two blocks at " << prev -> getStartAddress() << " and " << it -> getStartAddress() << endl;
                            it -> setsize(it -> getsize() + prev -> getsize());
                            it->setStartAddress(prev->getStartAddress());
                            Avail.erase(prev);
                            continue;
                        }
                    }
                    cout << "Success in deallocating  a process" << endl;
                }
                else
                cout << "Unable to comply as the indicated block cannot be found." << endl;
                printLists(Avail, Inuse);       //Print the contents of Available and Inuse list
            }
            //End if ? is encountered
            else if(x == "?")
            {
               break;
            }
        }
        myfile.close();     //Close the file
    }
    else
        cout << "Unable to open file";      //Print a message if file is not accessable
}

//Function to load a memory block and then push it to the Inuse list
//while reducing the appropriate size from available memory list
//using the best fit algorithm
bool loadTransaction(string PID, string BID, int blockSize)
{
    Memory compBlock, BestBlock;
    BestBlock.setsize(-1);      //Set the size as -1
    list<Memory>::iterator it = Avail.begin();      //Create an iterator
    for(; it != Avail.end(); it++)
    {
        //If the iterator's size is greater than the provided block size
        if(it -> getsize() >= blockSize)
        {
            //If the size of the best block is -1 the change it
            if(BestBlock.getsize() == -1)
            {
                BestBlock.setsize(it -> getsize());
                BestBlock.setStartAddress(it -> getStartAddress());
            }
            //If the size of the best block's size is greater than the iterator's size
            else if (BestBlock.getsize() > it -> getsize())
            {
                BestBlock.setsize(it -> getsize());
                BestBlock.setStartAddress(it -> getStartAddress());
            }

        }
    }
    //Iterate through the available list
    for(list<Memory>::iterator itt = Avail.begin(); itt != Avail.end(); itt++)
    {
        //Check and put the new memory block in Inuse and reduce the appropriate size from Avail
        if(itt -> getStartAddress() == BestBlock.getStartAddress()&& itt -> getsize() == BestBlock.getsize())
        {
                Memory newBlock(itt -> getStartAddress(), blockSize, PID, BID);
                cout << "Found a block of size " << itt -> getsize() << endl;
                itt -> setsize(itt -> getsize() - blockSize);
                itt -> setStartAddress(itt ->getStartAddress() + blockSize);
                Inuse.push_front(newBlock);
                return true;
        }
        else if(itt -> getsize() == compBlock.getsize())
        {
            Avail.remove(*itt);
        }
    }
    return false;
}

//Function to terminate a process by removing all the blocks
//in the process and then removing it from the Inuse list
bool terminateProcess(string PID)
{
    vector<string> BLOCKID;     //Vector to save all the values
    for(list<Memory>::iterator itt = Inuse.begin(); itt != Inuse.end(); itt++)
    {
        if(itt->getprocessID() == PID)
        {
            BLOCKID.push_back(itt -> getblockID());     //push the valuse of block ID in the vector
        }
    }

    //Iterate throgh the vector
    for(vector<string>::iterator t=BLOCKID.begin(); t!=BLOCKID.end(); ++t)
    {
        deallocateBlock(PID, *t);       //Call the deallocate function
    }

    //If the vector is not empty
    if(!BLOCKID.empty())
        return true;
    return false;
}

//Function to allocate memory to a new block with the same process ID
//using the first fit algorithm
bool allocateTransaction(string PID, string BID, int blockSize)
{
    Memory compBlock, BestBlock;
    BestBlock.setsize(-1);      //Set the size as -1
    list<Memory>::iterator it = Avail.begin();      //Create an iterator
    for(; it != Avail.end(); it++)
    {
        //If the iterator's size is greater than the provided block size
        if(it -> getsize() >= blockSize)
        {
            //If the size of the best block is -1 the change it
            if(BestBlock.getsize() == -1)
            {
                BestBlock.setsize(it -> getsize());
                BestBlock.setStartAddress(it -> getStartAddress());
            }
            //If the size of the best block's size is greater than the iterator's size
            else if (BestBlock.getsize() > it -> getsize())
            {
                BestBlock.setsize(it -> getsize());
                BestBlock.setStartAddress(it -> getStartAddress());
            }

        }
    }
    //Iterate through the available list
    for(list<Memory>::iterator itt = Avail.begin(); itt != Avail.end(); itt++)
    {
        //Check and put the new memory block in Inuse and reduce the appropriate size from Avail
        if(itt -> getStartAddress() == BestBlock.getStartAddress()&& itt -> getsize() == BestBlock.getsize())
        {
                Memory newBlock(itt -> getStartAddress(), blockSize, PID, BID);
                cout << "Found a block of size " << itt -> getsize() << endl;
                itt -> setsize(itt -> getsize() - blockSize);
                itt -> setStartAddress(itt ->getStartAddress() + blockSize);
                Inuse.push_front(newBlock);
                return true;
        }
        else if(itt -> getsize() == compBlock.getsize())
        {
            Avail.remove(*itt);
        }
    }
    return false;
}

//Deallocate a single Block with the given Process ID and the Block ID
bool deallocateBlock(string PID, string BID)
{

    list<Memory>::iterator it = Inuse.begin();      //Create an Iterator to the list
    list<Memory> temp;      //Create a temp list of memory blocks
    while(it != Inuse.end())
    {
        //If hte Process Id and the Block ID match the iterator's Process ID and the Block ID
        if(it -> getprocessID() == PID && it -> getblockID() == BID)
        {
            temp.push_back(*it);        //Push the memory block into the temp list
            it = Inuse.erase(it);       //Erase the memory block from the list
        }
        else
            it++;       //Increment the iterator
    }

    //Iterate throught the Avaiable list
    for(list<Memory>::iterator it = Avail.begin(); it != Avail.end(); it++)
    {
        int b = it->getStartAddress();
        //Iterate through the temp list
        for(list<Memory>::iterator itt = temp.begin(); itt != temp.end(); itt++)
        {
            int c = itt->getStartAddress();
            Memory DeallocateBlock = *itt;
            //If the available memory block's start address is greater than the temp memory block's start address
            if(b >= c)
            {
                Avail.insert(it, DeallocateBlock);      //Insert into the available memory list at position it the block DeallocateBlock
                return true;        //Return true
            }
        }
    }

    return false;
}

//Function that traverses through the given file and allocate the
//memory on first come first serve basis
void first_fit(ifstream &myfile)
{
    //Check if the file is open
    if(myfile.is_open())
    {
        //Create variables
        string line;
        vector<string> v;
        string word;

        //Get each word from the file and put it in the vector
        while(getline(myfile,line))
        {
            istringstream iss(line);
            for(word; iss >> word; )
            v.push_back(word);
        }
        //Iterate through the vector
        for (vector<string>::iterator t=v.begin(); t!=v.end(); ++t)
        {
            string x = *t;
            //If the load transaction is selected
            if(x == "L")
            {
                //Assign variables
                int Address, blockSize;
                string PID,BID;
                t++; PID = *t;
                t++; blockSize = stoi(*t);
                t++; BID = *t;
                cout << "Transaction:  request to load process " << PID <<", block ID " << BID << " using " << blockSize << endl;
                //Perform the load algorithm
                if (loadFirstTransaction(PID,BID,blockSize))
                    cout << "Success in allocating a block" << endl;
                printLists(Avail,Inuse);        //Print the contents of Available and Inuse list
            }
            //If terminate process is selected
            else if(x == "T")
            {
                t++;
                cout << "Transaction:  request to terminate process " << *t << endl;
                //Perform the terminate algorithm
                if(terminateProcess(*t))
                {
                    for(list<Memory>::iterator it = Avail.begin(), prev = Avail.end(); it != Avail.end(); prev = it, ++it)
                    {
                        //Check if the memory of consicutive blocks is less than 4 MB
                        if(prev != Avail.end() && (it -> getsize() + prev -> getsize()) < 4*MB)
                        {
                            //Perform a meage of the blocks that are less than 4 MB
                            cout << "Merging two blocks at " << prev -> getStartAddress() << " and " << it -> getStartAddress() << endl;
                            it -> setsize(it -> getsize() + prev -> getsize());
                            it->setStartAddress(prev->getStartAddress());
                            Avail.erase(prev);
                            continue;
                        }
                    }
                    cout << "Success in terminating a process" << endl;
                }
                else
                cout << "Unable to comply as the indicated process could not be found." << endl;
                printLists(Avail,Inuse);        //Print the contents of Available and Inuse list
            }
            //If Allocate block is selected
            else if(x == "A")
            {
                //Assign variables
                int Address, blockSize;
                string PID,BID;
                t++; PID = *t;
                t++; blockSize = stoi(*t);
                t++; BID = *t;
                cout << "Transaction:  request to allocate " << blockSize <<" bytes for process " << PID << ", block ID: " << BID << endl;
                //Perform the allocate algorithm
                if (allocateFirstTransaction(PID,BID,blockSize))
                    cout << "Success in allocating a block" << endl;
                else
                    cout << "Unable to comply as the indicated process could not be found." << endl;
                printLists(Avail,Inuse);        //Print the contents of Available and Inuse list
            }
            //If Deallocate block is selected
            else if(x == "D")
            {
                //Assign variables
                string PID, BID;
                cout<<*t<< " ";
                t++; PID = *t; cout<<PID<< " ";
                t++; BID = *t; cout<<BID<<endl;
                //Perform the deallocate algorithm
                if(deallocateBlock(PID, BID))
                {
                    for(list<Memory>::iterator it = Avail.begin(), prev = Avail.end(); it != Avail.end(); prev = it, ++it)
                    {
                        //Check if the memory of consicutive blocks is less than 4 MB
                        if(prev != Avail.end() && (it -> getsize() + prev -> getsize()) < 4*MB)
                        {
                            //Perform a meage of the blocks that are less than 4 MB
                            cout << "Merging two blocks at " << prev -> getStartAddress() << " and " << it -> getStartAddress() << endl;
                            it -> setsize(it -> getsize() + prev -> getsize());
                            it->setStartAddress(prev->getStartAddress());
                            Avail.erase(prev);
                            continue;
                        }
                    }
                    cout << "Success in deallocating  a process" << endl;
                }
                else
                cout << "Unable to comply as the indicated block cannot be found." << endl;
                printLists(Avail, Inuse);       //Print the contents of Available and Inuse list
            }
            //End if ? is encountered
            else if(x == "?")
            {
               break;
            }
        }
        myfile.close();     //Close the file
    }
    else cout << "Unable to open file";     //Print a message if file is not accessable
}
//Function to terminate a process by removing all the blocks
//in the process and then removing it from the Inuse list
//using the first fit algorithm
bool loadFirstTransaction(string PID,string BID,int blockSize)
{
    Memory compBlock;
    //Iterate through the Available memory list
    for(list<Memory>::iterator itt = Avail.begin(); itt != Avail.end(); itt++)
    {
        //Check for the first memory block that has a greater size than the block size provided
        if(itt -> getsize() >= blockSize)
        {
            Memory newBlock(itt -> getStartAddress(), blockSize, PID, BID);
            cout << "Found a block of size " << itt -> getsize() << endl;
            itt -> setsize(itt -> getsize() - blockSize);
            itt -> setStartAddress(itt ->getStartAddress() + blockSize);
            Inuse.push_front(newBlock);
            return true;
        }
        else if(itt -> getsize() == compBlock.getsize())
        {
            Avail.remove(*itt);
        }
    }
    return false;
}

//Function to allocate memory to a new block with the same process ID
//using the first fit algorithm
bool allocateFirstTransaction(string PID,string BID,int blockSize)
{
    Memory compBlock;
    //Iterate through the available memory list
    for(list<Memory>::iterator itt = Avail.begin(); itt != Avail.end(); itt++)
    {
        //Check for the first memory block that has a greater size than the block size provided
        if(itt -> getsize() >= blockSize)
        {
            Memory newBlock(itt -> getStartAddress(), blockSize, PID, BID);
            cout << "Found a block of size " << itt -> getsize() << endl;
            itt -> setsize(itt -> getsize() - blockSize);
            itt -> setStartAddress(itt ->getStartAddress() + blockSize);
            Inuse.push_front(newBlock);
            return true;
        }
        else if(itt -> getsize() == compBlock.getsize())
        {
            Avail.remove(*itt);
        }
    }
    return false;
}

//Function to populate the available list with the
//required memory blocks
list<Memory> populateAvail(list<Memory> Avail)
{
    //Creating the memory block objects
    Memory OneMB;
    Memory TwoMB;
    Memory FourMB;
    //setting the first block
    OneMB.setsize(1*MB);
    OneMB.setStartAddress(3*MB);        //Making sure that the first block points to the start address of 3MB
    Avail.push_back(OneMB);         //Pushing to the available list

    TwoMB.setsize(2*MB);
    TwoMB.setStartAddress(3*MB + 1*MB);
    Avail.push_back(TwoMB);         //Pushing to the available list
    TwoMB.setStartAddress(4*MB + 2*MB);
    Avail.push_back(TwoMB);         //Pushing to the available list

    FourMB.setsize(4*MB);
    FourMB.setStartAddress(6*MB + 2*MB);
    Avail.push_back(FourMB);        //Pushing to the available list
    FourMB.setStartAddress(8*MB + 4*MB);
    Avail.push_back(FourMB);        //Pushing to the available list

    return Avail;                   //Returning the list
}

//Function to print both the available list and the inuse list
void printLists(list<Memory> Avail, list<Memory> Inuse)
{
    cout << endl << "List of available blocks" << endl;
    printAvailList(Avail);      //Function call to print the available list
    cout << endl << "List of blocks in use" << endl;
    printInuseList(Inuse);      //Function call to print the Inuse list
}

//Function to print the Available list
void printAvailList(list<Memory> PopulatedList)
{
    list<Memory>::iterator itt = PopulatedList.begin();     //Create an iterator
    int x = 0;
    if(itt != PopulatedList.end())      //Check that the end of the list isn't reached
    {
        //Iterate through the populated list
        for(itt = PopulatedList.begin(); itt != PopulatedList.end(); itt++)
        {
            Memory temp = *itt;
            cout << "Start Address =  "<< temp.getStartAddress() << " Size = " << temp.getsize() << endl;       //Print data
            x += temp.getsize();        //Sum of all sizes of the memory blocks in the available list
        }
    }
    else
        cout << "(none)" << endl;       //If the list is empty
    cout << "Total size of the list = " << x << endl;       //Print the total size of the list
}

//Function to print the Inuse list
void printInuseList(list<Memory> PopulatedList)
{
    list<Memory>::iterator itt = PopulatedList.begin();     //Create an iterator
    int x = 0;
    if(itt != PopulatedList.end())      //Check that the end of the list isn't reached
    {
        //Iterate through the populated list
        for(itt = PopulatedList.begin(); itt != PopulatedList.end(); itt++)
        {
            Memory temp = *itt;
            cout << "Start Address =  "<< temp.getStartAddress() << " Size = " << temp.getsize() << " Process ID = " << temp.getprocessID() << " Block ID = " << temp.getblockID()<< endl; //Print data
            x += temp.getsize();        //Sum of all sizes of the memory blocks in the available list
        }
    }
    else
        cout << "(none)" << endl;       //If the list is empty
    cout << "Total size of the list = " << x << endl << endl;       //Print the total size of the list
}
