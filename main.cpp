//
//  main.cpp
//  Petit Cafe Du Monde Sales Record Book
//
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
#include "Order.h"
#include "HashTable.h"
#include "BinarySearchTree.h"
#include <algorithm>

using namespace std;

//Prototypes
void searchManager(const BinarySearchTree<Order> &orderQue);
// deleteManager
// newOrderManager
void printSalesReport();
void displayOption();
void displayBST(BinarySearchTree<Order> &list);
void displayHash(HashTable<Order> orderHash);
void buildArchive(const string &filename, HashTable<Order> &arc, BinarySearchTree<Order> &list);
void hashSearch(HashTable<Order> &orderHash);
void addOrder(HashTable<Order> &orderHash, BinarySearchTree<Order> &list);
void deleteOrder(HashTable<Order> &orderHash, BinarySearchTree<Order> &list);
void iDisplay(Order &item, int level);
void hDisplay (Order &item);
void outDisplay (Order &item);
void displayHeaderReciept();
void BSTsearch(BinarySearchTree<Order> &list);
void writeToFile(BinarySearchTree<Order> &list);
void displayBSTindent(BinarySearchTree<Order> &list);
void coffeeCalculater(string drinkName, Coffee & pCoffee, Order &newReciept);
void displayHashStats(Order orderHash);
void bstDisplayManager(BinarySearchTree<Order> &list);
void displayStats(HashTable<Order> orderHash);

//To be implemented:- Works just like the hashSearch, except using the orderNum to display the person's name and order data
void hashOrderNumSearch(HashTable<Order> &orderHash);

//Coffee Choices-------------------
Coffee capp("Cappuccino", 3.30);
Coffee america("Americano", 2.80);
Coffee latte("Latte", 3.30);
Coffee espress("Espresso", 2.00);
//---------------------------------

//Defining OutputFile filename-----
string saveFile = "coffeeOut.txt";
//---------------------------------
 //Holds the amount of collisons;
int numCollisions;

int main()
{
    string fileName = "Coffee.txt";
    int userOption;
    
    //cout << "What is the input file's name? "; <-- Eliminated entering of .txt file for ease of use
    //cin >> fileName;
    BinarySearchTree<Order> orderList;  // <-- Replace with a BST?
    HashTable<Order> orderHash;
    
    buildArchive(fileName, orderHash, orderList);
    
    //Displaying menu
    displayOption();
    cin >> userOption;
    while (userOption != 0)
    {
        //Please change the switch cases to correspond with those designated in displayOption()
        switch (userOption)
        {
            case 1 : addOrder(orderHash, orderList);
                break;
            case 2 :  bstDisplayManager(orderList); //displayBST(orderList); // Display order in sorted order from the BST
                break;
            case 3 : deleteOrder(orderHash, orderList); // Delete Orders
                break;
            case 4 : hashSearch(orderHash);// Search Hash
                break;
            case 5 : displayStats(orderHash);//Print Statistics
                break;
            case 6: writeToFile(orderList); //Save to File
                break;
            case 7: displayBSTindent(orderList);// hidden function displays indented BST
                break;
            case 9: displayOption(); // Help Menu
                break;
            default: cout << "Please try again\n";
        }
    
        cin >>  userOption;
    }
    
    return 0;
    
}
//**************************************************
// Main UI of the program
//**************************************************
void displayOption()
{
    cout << "**************************************" << endl;
    cout <<  "Petit Cafe Du Monde Sales Record Book" << endl;
    cout << "**************************************" << endl;
    cout << "            ~Main Menu~                  " << endl;
    cout << "Please select from the following options:\n" << endl;
    cout << "1.Add new Order\n" << "2.Print Sales Report" << endl;
    cout << "3.Delete an Order\n" << "4.Search for an Order\n" << "5.Display Statistics\n"
         << "6.Save File\n" << "Enter 0 to quit Program or Enter 9 for help\n";
    cout << "************************************" << endl;
    cout << "Enter Option choice: ";
}

//**************************************************
// Display Header for Reciept(name/ points/ coffee etc
//**************************************************
void displayHeaderReciept()
{
    cout << endl << " ====    ======     ========   ======    ========   ====== \n";
    cout << " Name    Points     Order      Price     Date       Order#" << endl;
    cout << " ====    ======     ========   ======    ========   ====== \n";

}


//Doesn't work (Must display the entire archive)
void displayHash(HashTable<Order> orderHash)
{
    displayHeaderReciept(); //Displays the Header for customer reciepts
    
    int hashCount = orderHash.getSize();
    
    while(hashCount != 0)
    {
        Order bob;
        
        cout << bob.getPersonName() << " " << bob.getPersonPoints() << " " << bob.getDrinkName() << " "
        << bob.getDrinkPrice() << "" << bob.getDate() << " " << bob.getOrderNum();
        hashCount--;
    }
    cout << endl << endl;
    
}

//**************************************************
// Display Order BST inorder
//**************************************************
void displayBST(BinarySearchTree<Order> &list)
{
    
    displayHeaderReciept(); // Displays the Header for customer reciepts
    //Horizontal display
    list.inOrder(hDisplay);
    cout << endl << endl;

}

//**************************************************
// Display Order BST in indented format ~Hidden Function
//**************************************************
void displayBSTindent(BinarySearchTree<Order> &list)
{
    
    //Indented
    list.printTree(iDisplay);
    cout << endl << endl << endl;
    
}

//**************************************************
// Hash index key used for organizing the HASH table
// inputs and deletes.
//**************************************************
int key_to_index(const Order &key, int size)
{
    string k = key.getOrderNum();
    int sum = 0;
    for (int i = 0; k[i]; i++)
        sum += k[i];
    return sum % size;
};

//**************************************************
// Builds archive for both the HASH Table and
// BST list by reading order data from a file
// and inputs them into BST and HASH Table
//**************************************************
void buildArchive(const string &filename, HashTable<Order> &arc, BinarySearchTree<Order> &list)
{
    //Coffee choices(Original spot, now at very beginning of program)
    /*
    Coffee capp("Cappuccino", 3.30);
    Coffee america("Americano", 2.80);
    Coffee latte("Latte", 3.30);
    Coffee espress("Espresso", 2.00);
     */
    //
    ifstream fin(filename);

    if(!fin)
    {
        cout << "Error opening the input file: \""<< filename << "\"" << endl;
        exit(EXIT_FAILURE);
    }

    int points;
    string name, drinkName, orderNum, date;
    while(fin >> orderNum)
    {
        fin >> date;
        fin.ignore(); // to ignore space in front of name
        getline(fin, name, ';');
        fin.ignore();
        getline(fin, drinkName, ';');
        fin.ignore();
        fin >> points;
        
        Customer person(name,points,0);
        Coffee pCoffee(drinkName,0);
        Order reciept(person, pCoffee, date, orderNum);
        
        coffeeCalculater(drinkName, pCoffee, reciept);
        
        //Debug to check Insert------------------------------------------------------------------------------------------
        //cout << "I am inserting " << reciept.getPersonName() << " who bought a " << reciept.getDrinkName() << endl;
        //arc.insert(reciept,key_to_index);
        //cout << person.getName() << " was inserted!" << endl;
        //---------------------------------------------------------------------------------------------------------------
        
        //Inserting the new recipt from file into both the HASH Table and BST
        arc.insert(reciept,key_to_index);
        list.insert(reciept);
    }
    fin.close();
}

//**************************************************
// Adds a new user order entered order to both
// the HASH table and BST
//**************************************************
void addOrder(HashTable<Order> &orderHash, BinarySearchTree<Order> &list)
{
    int points;
    string name, drinkName, orderNum, date;
    bool foundDup = false;
    
    // Getting new order data
    cout << endl << endl <<  "~~~~~~~~ Add order Menu ~~~~~~~~" << endl << endl;
    cout << "Enter the Customers name: ";
    cin >> name;
    cout << "Enter the Customers drink name: ";
    cin >> drinkName;
    cout << "Enter date of order (Month/Day/Year): ";
    cin >> date;
    cout << "Enter the Customers 5 digit order number: ";
    cin >> orderNum;
    cout << "Enter the Customers current points: ";
    cin >> points;
    
    Coffee pCoffee(drinkName,0);
    Customer person(name,points,0);
    Order newReciept(person, pCoffee, date, orderNum);
    
    //Customer person(name,0,0);
    Order found;
    Coffee drink;
    Order key(person, drink, " ", " ");
    
    if(drinkName == "Cappuccino" || drinkName == "cappuccino"){
        pCoffee = capp;
        newReciept.setCoffee(pCoffee);
    }
    
    else if(drinkName == "Americano" || drinkName == "americano"){
        pCoffee = america;
        newReciept.setCoffee(pCoffee);
    }
    
    else if(drinkName == "Latte" || drinkName == "latte"){
        pCoffee = latte;
        newReciept.setCoffee(pCoffee);
    }
    
    else if(drinkName == "Espresso" || drinkName == "espresso"){
        pCoffee = espress;
        newReciept.setCoffee(pCoffee);
    }
    
    //Adding new order pointers in both BST and Hash table !!** Hash insert still needs to be fixed!!**
    for(int i = 0; i < orderHash.getCount(); i++)
    {
        if(orderHash.getHashAry(i).getItem().getOrderNum() == orderNum)
        {
            cout << "Error:: Duplicate order Number entered; Please try again" << endl;
            foundDup = true;
            break;
        }
    }
    
    if(foundDup == false)
    {
        if (orderHash.insert(newReciept, key_to_index) && list.insert(newReciept))
        {
            cout << endl <<  person.getName() << "'s  reciept has been entered succefully" << endl << endl;
        }
        else
        {
                cout << "Could not enter new order number try again." << endl;
        }
    }
}

// Searches the hash table archive for any given transaction, name or order number must be given
void hashSearch(HashTable<Order> &orderHash){
    int numOfDuplicates = 1;
    char userChoice;
    
    cout << "\nWhich key would you like search the archive by? Name or Order Number?(N/O): ";
    cin >> userChoice;
    
    //If user enters 'N', the archive will be searched by name, else if 'O' is entered, hashOrderNumSearch is called(which has the Primary Key Manager interface)
    if(userChoice == 'n' || userChoice == 'N'){
        string name;
        Coffee drink;
        
        cout << "\nWelcome to the Secondary Key Search Manager(Non-Unique)!" << endl;
        cout << "--------------------------------------------------------" << endl;
        
        cout << "Enter name or Q to quit: ";
        cin >> name;
        while ( name != "Q" )
        {
            Customer person(name,0,0);
            Order found;
            Order key(person, drink, " ", " ");
            
            //STAGED FOR SAMPLE OUTPUT: need to implement a system that counts the numOfDuplicates and updates this number
            //The data printed out must correspond to this (i.e. "Found 2 orders to a Samantha" needs to print out those two different orders )
            
            for(int i = 0; i < orderHash.getCount(); i++)
            {
                if(orderHash.getHashAry(i).getItem().getPersonName() == name)
                {
                    //cout << "Error:: Duplicate order Number entered; Please try again" << endl;
                    numOfDuplicates++;
                    break;
                }
            }
            
                   
            if ((orderHash.search(found, key, key_to_index)) != -1)
            {
                /*
                 Debug Code
                cout << "Found test: " << endl;
                cout << found.getDrinkName() << endl;
                cout << found.getPersonName() << endl;
                cout << found.getDate() << endl;
                cout << found.getOrderNum() << endl;
                 */
                // First prints out how many were found with the specific key
                //Prints out the data found in the hash in an organized format(Name, Drink name, date, and price)
                if(numOfDuplicates > 1)
                {
                    displayHeaderReciept();
                    cout << endl << "Found " << numOfDuplicates << " orders to a " << found.getPersonName() << "!" << endl;
                    for(int i = 0; i < orderHash.getCount(); i++)
                    {
                        
                        if(orderHash.getHashAry(i).getItem().getPersonName() == name)
                        {
                            cout << orderHash.getHashAry(i).getItem()<< endl;
                        }
                    }
                    
                }
                
                else
                {
                    cout << endl << "Found " << numOfDuplicates << " order to a " << found.getPersonName() << "!" << endl;
                    hDisplay(found);
                }
                //cout << found.getPersonName() << " ordered a " << found.getDrinkName() << " on " << found.getDate()
                    //<< " (Price: $" << found.getDrinkPrice() << ")" << endl;
                 
            }
            else
            {
                cout << endl << name << " not found!" << endl;
            }
            cout << "\nEnter name or Q to quit: ";
            cin >> name;
        }
    }
    
    else if(userChoice == 'o' || userChoice == 'O')
    {
        //cout << "To be continued" << endl; // need to implement a OrderNum searcher here
        hashOrderNumSearch(orderHash);
    }
}

//**************************************************
// Deletes an user entered reciept from both the Hash Table
// and BST order list.
//**************************************************
// Insert and Delete <== Queue (John Yun)
void deleteOrder(HashTable<Order> &orderHash, BinarySearchTree<Order> &list)
{
    string delOrderNum, person; // Holds the deleted order# and name
    Order delReciptCopy, delRecipt;
    
    
    cout << "Enter customers order number; " << endl;
    cin >> delOrderNum;
    delRecipt.setOrderNum(delOrderNum);
    
    // deleting order pointer from hash table
    /*
    orderHash.remove(delReciptCopy, delRecipt, key_to_index);
    // deleting order pointer form BST
    */
    orderHash.remove(delReciptCopy, delRecipt, key_to_index);
    
    list.remove(delReciptCopy);
    
}

//**************************************************
// Display's the list of Customers Horizontaly
//**************************************************
void hDisplay (Order &item)
{
    cout << left;
    cout << " " << setw(9) << item.getPersonName();
    cout << " " << setw(8) << item.getPersonPoints();
    cout << " " << setw(12) << item.getDrinkName();
    cout << " " << setw(7) << item.getDrinkPrice();
    cout << " " << setw(10) << item.getDate();
    cout << " " << setw(8) << item.getOrderNum();
    cout <<endl;
}

//**************************************************
// Display's the list of Customers Horizontaly into
// an outpute file
//**************************************************
void outDisplay (Order &item)
{
    ofstream outputFile; // adding Report to filname
    //validation
    if(outputFile.fail()){
        cout << "Error opening the output file: " << saveFile << ".\n";
            exit(EXIT_FAILURE);
    }
    outputFile.open(saveFile, ios::out|ios::app); // ios::out|ios::app <<<<< used to append the infromation to the file
    
    outputFile << endl;
    outputFile << item.getOrderNum();
    outputFile << " " << item.getDate();
    outputFile << " " << item.getPersonName() << ";";
    outputFile << " " << item.getDrinkName() << ";";
    outputFile << " " << item.getPersonPoints();
    outputFile << endl;
}

//*****************************************************
// A Display's the list of Customers in an indented format
//******************************************************
void iDisplay(Order &item, int level)
{
    for (int i = 1; i < level; i++)
        cout << "..";
    cout << level << "). " << item.getPersonName() << endl;
};

//*****************************************************
// Writes all reciept Data to a file using the secondary
// key from the BST
//******************************************************
void writeToFile(BinarySearchTree<Order> &list)
{
    //Writing to file
    ofstream outputFile;
    outputFile.open(saveFile);
    
    outputFile << " ====    ======     ========   ======    ========   ====== \n";
    outputFile << " Name    Points     Order      Price     Date       Order#" << endl;
    outputFile << " ====    ======     ========   ======    ========   ====== \n";
    
    
    list.inOrder(outDisplay);
}

//*****************************************************
// This function caclulates the Coffee cost
//******************************************************
void coffeeCalculater(string drinkName, Coffee& pCoffee, Order &newReciept)
{
    
    if(drinkName == "Cappuccino"){
        pCoffee = capp;
        newReciept.setCoffee(pCoffee);
    }
    
    else if(drinkName == "Americano"){
        pCoffee = america;
        newReciept.setCoffee(pCoffee);
    }
    
    else if(drinkName == "Latte"){
        pCoffee = latte;
        newReciept.setCoffee(pCoffee);
    }
    
    else if(drinkName == "Espresso"){
        pCoffee = espress;
        newReciept.setCoffee(pCoffee);
    }
}

//*******************************************************
// This function display the Team Member ~~Hidden display~
//********************************************************
void displayTeam()
{
    cout << "Team Leader: Sohailahmed Syed" << endl;
    cout << "Team Members: John Yun, Larry Shields" << endl;
}

//*******************************************************
// This function display the Hash Table Statistics
//********************************************************
void displayStats(HashTable<Order> orderHash)
{
    //orderHash.getHashAry()->setNoCollisions(numCollisions);
    
    for(int i = 0; i < orderHash.getCount(); i++)
    {
        numCollisions += orderHash.getHashAry(i).getNoCollisions();
    }
    
    cout << "Load Factor: " << orderHash.getLoadFactor() << endl;
    //cout << "Number of Collisions: " << orderHash.getHashAry()->getNoCollisions() << endl;
    cout << "Number of Collisions: " << numCollisions << endl;
    cout << endl;
    
   //cout << orderHash.getHashAry()->getNoCollisions() << endl;

}

//New
void bstDisplayManager(BinarySearchTree<Order> &list)
{
    int userChoice;
    string name;
    Coffee drink;
    
    cout << "--------------------------------------------------------" << endl;
    cout << "\nWelcome to the Display Manager!" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    cout << "Please choose one of the following report viewing options" << endl;
    cout << "1. Standard Report View" << endl;
    cout << "2. Hierachical(Indented) View" << endl;
    cout << "\nEnter Option: " << endl;
    cin >> userChoice;
    
    if(userChoice == 1)
    {
        cout << "Standard Sales Report" << endl;
        cout << "--------------------------------------------------------" << endl;
        displayBST(list);
        cout << endl;
    }
    
    else if(userChoice == 2)
    {
        cout << "Indented Sales Report" << endl;
        cout << "--------------------------------------------------------" << endl;
        list.printTree(iDisplay);
        cout << endl;
    }
    
}

//New
void hashOrderNumSearch(HashTable<Order> &orderHash)
{
    string orderNum;
    string name;
    Coffee drink;
    
    cout << "\nWelcome to the Primary Key Search Manager(Unique)!" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    cout << "Enter order number or Q to quit: ";
    cin >> orderNum;
    
    while (orderNum != "Q" )
    {
        Customer person(name,0,0);
        Order found;
        Order key(person, drink, " ", orderNum);
        
        int numOfDuplicates = 1;
        
        if ((orderHash.orderSearch(found, key, key_to_index)) != -1)
        {
            /*
             Debug Code
             cout << "Found test: " << endl;
             cout << found.getDrinkName() << endl;
             cout << found.getPersonName() << endl;
             cout << found.getDate() << endl;
             cout << found.getOrderNum() << endl;
             */
            // First prints out how many were found with the specific key
            //Prints out the data found in the hash in an organized format(Name, Drink name, date, and price)
            cout << endl << "Found " << numOfDuplicates << " order to a " << found.getPersonName() << "!" << endl;
            cout << found.getPersonName() << " ordered a " << found.getDrinkName() << " on " << found.getDate()
            << " (Price: $" << found.getDrinkPrice() << ")" << endl;
        }
        else
        {
            cout << endl << orderNum << " not found!" << endl;
        }
        
    cout << "\nEnter order number or Q to quit: ";
    cin >> orderNum;
    
}
}





