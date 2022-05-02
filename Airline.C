#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//NUM_SEATS is the number of seats on the plane,
#define NUM_SEATS 60
//SEATS_ROW is the number of seats in a row,
#define SEATS_ROW 6

//Structure for seat spot on plane
struct seat{
  int num=-1;
  int ownerId=-1;
}noSeat,seats[NUM_SEATS];
//Structure for customer, id is set to numCustomers at objects creation
struct customer{
  char userName[500];
  char *pUserName = userName;
  int id=-1;
  int numSeats=0;
  struct seat* spot=(struct seat*)malloc(1*sizeof(seat));
}noCustomer,customers[50];
/*
  input: None

  output: None

  remarks: Sets the seat numbers
*/
void Init(){
  for(int x=0;x<NUM_SEATS;x++){
    seats[x].num=x+1;
  }
  return;
}
void clearScreen(){
  system("cls");
  return;
}
/*
  input: None

  output: None

  remarks:
    Prints the formatted seats
    Divided into rows with corresponding letter with break in middle
    i.e.
    A  123  456
    B  123  456
*/
void PrintSeats(){
  char row='A';
  printf("Seats\n---------------------");
  for(int x=0;x<(NUM_SEATS);x++){
    if((x)%SEATS_ROW==0){
      printf("\n%c:  ",row);
      row+=1;
    }
    else if((x%6)%3==0){
      printf("  ");
    }

    if(seats[x].ownerId==-1){
      if(seats[x].num%SEATS_ROW!=0){
        printf("%d ",(seats[x].num%SEATS_ROW));
      }
      else{
        printf("%d",(SEATS_ROW));
      }
    }
    else{
      printf("x ");
    }
  }
  printf("\n");
  return;
}
/*
  input: The seat's number from seat struct

  output: Pointer to char array containing (letter, number) combination

  remarks:
    Converts seat number to (letter, number) combination
    i.e. when SEATS_ROW=6,
    1->A1
    9->B3
*/
char * NumToSpot(int trueSeatNum){
  int mixedSeatNum=(trueSeatNum%6);
  char letter;
  char seatChr;
  char* spot = (char *)malloc(3*sizeof(char));
  if(mixedSeatNum==0){
    mixedSeatNum=6;
  }
  trueSeatNum-=mixedSeatNum;
  trueSeatNum=(trueSeatNum/6)+'A';
  letter=trueSeatNum;
  seatChr=mixedSeatNum+'0';
  spot[0]=letter;
  spot[1]=seatChr;
  spot[2]='\0';
  return spot;
}
/*
  input: Pointer to user object (based on customer struct )
::
  output: Returns 0 if user has a seat, -1 if user doesn't

  remarks: Prints the seat owned by the user
*/
int GetSeats(customer* pUser){
  if(pUser->spot==NULL||pUser->spot[0].ownerId==-1){
    printf("No reserved seats\n");
    return -1;
  }
  char* seatMixedPos;
  for(int x=0;x<pUser->numSeats;x++){

    seatMixedPos = NumToSpot(pUser->spot[x].num);
    printf("Reserved seat %s\n",seatMixedPos);
  }
  free(seatMixedPos);
  return 0;
}
/*
  input: Pointer to user object (based on customer struct)

  output: None

  remarks:  Lets user reserve a seat
*/
void ReserveSeat(customer* pUser){
  clearScreen();
  printf("User: %s\n",pUser->userName);
  int newSeats;
  int count;
  int seatNum;
  int rowSpot;
  char row;

  while(true){
    printf("How many seats would you like to reserve: ");
    scanf("%d",&newSeats);
    pUser->spot=(struct seat *)realloc(pUser->spot,(pUser->numSeats+newSeats)*sizeof(seat));
    break;
  }

  for(int x=0;x<newSeats;x++)
  {
    PrintSeats();

    printf("Enter which seat you would like: \n");
    scanf(" %c",&row);
    seatNum=((row-'A')*6);
    scanf("%d",&rowSpot);
    seatNum+=rowSpot;
    if(seats[seatNum-1].ownerId==-1){
      pUser->spot[pUser->numSeats]=seats[seatNum-1];

      pUser->spot[pUser->numSeats].ownerId=pUser->id;

      seats[seatNum-1].ownerId=pUser->id;
      pUser->numSeats+=1;

    }
    else{
      printf("This seat is already taken, please enter another seat\n");
      x--;
    }
    clearScreen();
    printf("User: %s\n",pUser->userName);
  }
  GetSeats(pUser);
  return;
}
/*
  input: Pointer to user object (based on customer struct)

  output: None

  remarks: Removes seat from user's account and marks as unowned
*/
void CancelSeats(customer* pUser){
  clearScreen();
  printf("User: %s\n",pUser->userName);
  bool found= false;
  char recVal[3];
  char row;
  int rowSpot;
  int seatNum;
  GetSeats(pUser);
  while(true){
    printf("Enter which seat you would like to cancel: (-1 to quit)\n");
    scanf("%s",recVal);
    if(strcmp("-1",recVal)==0){
      return;
    }
    else{
      row=recVal[0];
      rowSpot=recVal[1];
    }
    seatNum=((row-'A')*6);
    seatNum+=rowSpot;
    seatNum-=48;
    if(seats[seatNum-1].ownerId==pUser->id){
      for(int x=0;x<pUser->numSeats-1;x++){
        if (pUser->spot[x].num==seatNum){
          pUser->spot[x].ownerId=-1;
          pUser->spot[x]=pUser->spot[x+1];
          found=true;
        }
        else if(found){
          pUser->spot[x]=pUser->spot[x+1];
        }
      }
      if (pUser->spot[pUser->numSeats-1].num==seatNum){
        pUser->spot[pUser->numSeats-1].ownerId=-1;
        pUser->spot[pUser->numSeats-1]=noSeat;
      }

      pUser->numSeats-=1;
      seats[seatNum-1].ownerId=-1;
      printf("Cancelled Seat\n");
      pUser->spot=(struct seat *)realloc(pUser->spot,pUser->numSeats*sizeof(seat));
    }

  }
  clearScreen();
}
/*
  input: User object (based on customer struct)

  output: None

  remarks:
    Allows user to manage and check status of their seat
    Leads to ReserveSeat, GetSeats, and CancelSeats functions

*/
void ManageSeats(customer user){
  printf("User: %s\n",user.userName);
  int choice;
  printf("1) Reserve seat 2) View seat 3) Cancel Seats 4) Quit\nEnter Option: \n");
  scanf("%1d",&choice);
  switch(choice){
    case 1:
      {
        ReserveSeat(&user);
        clearScreen();
        break;
      }
    case 2:
      {
        GetSeats(&user);

        break;
      }
    case 3:
      {
        CancelSeats(&user);
        clearScreen();
        break;
      }
    case 4:
      {
        clearScreen();
        printf("Exiting program");
        free(user.spot);
        exit(0);
      }
    default:
    {
      printf("Enter a correct input number\n");
      break;
    }
  }
  return ManageSeats(user);
}
/*
  input: Num Accounts int (holds the number of accounts created)

  output:
    Customer object (based on customer struct)
    noCustomer Object if enter -1 (default Customer values)

  remarks: Allows user to log in, enter -1 to exit
*/
customer LogIn(int numAccounts){
  char userInput[500];
  char exitVal[3]="-1";
  printf("Enter Username (-1 to exit): \n");
  while (true){
    scanf("%3s",userInput);
    if(strcmp(exitVal,userInput)==0){
      return noCustomer;
    }
    for (int x=0;x<numAccounts;x++){
      if(strcmp(customers[x].userName,userInput)==0){
        printf("Logging in %s\n",customers[x].userName);
        return customers[x];
      }
    }
printf("Couldn't find username. Please try again.");
  }
}
/*
  input: Pointer to Num Accounts int (holds the number of accounts created)

  output: Customer object (based on customer struct)

  remarks: Allows user to create a new account, asks for username
*/
customer CreateAccount(int* pNumAccounts){
  char exitVal[3]="-1";
  char userInput[500];
  customers[*pNumAccounts].id=*pNumAccounts;

  printf("Enter Username (-1 to exit): \n");
  scanf("%500s",userInput);
  if(strcmp(exitVal,userInput)==0){
    return noCustomer;
  }
  strcpy(customers[*pNumAccounts].userName,userInput);
  printf("Username: %s, ID: %d\n",customers[*pNumAccounts].userName,customers[*pNumAccounts].id);
  customers[*pNumAccounts].spot[0]=noSeat;
  *pNumAccounts+=1;

  clearScreen();
  return customers[*pNumAccounts-1];
}
/*
  input:
    Pointer to Num Accounts int (holds the number of acounts created)
    Const char array name (user's username) with max chars 499 not including '\0'

  output: Customer object (based on customer struct)

  remarks: Creates account, given username
*/
customer CreateAccount(int* pNumAccounts, const char name[500]){
  customers[*pNumAccounts].id=*pNumAccounts;
  strcpy(customers[*pNumAccounts].userName,name);

  printf("Username: %s, ID: %d\n",customers[*pNumAccounts].userName,customers[*pNumAccounts].id);
  customers[*pNumAccounts].spot[0]=noSeat;

  *pNumAccounts+=1;
  return customers[*pNumAccounts];
}
/*
  input: Pointer to Num Accounts int (holds the number of accounts created)

  output: Customer object (based on customer struct)

  remarks: Gets the account, leads to LogIn and CreateAccount functions
*/
customer GetAccount(int* pNumAccounts){
  int choice;
  customer user;
  printf("1) Log In 2) Create Account 3) Quit\nEnter Option: \n");
  scanf("%1d",&choice);
  switch(choice){
    case 1:
      {
        user=LogIn(*pNumAccounts);
        if(user.id==-1){
          break;
        }
        return user;
      }
    case 2:
      {
        user=CreateAccount(pNumAccounts);
        if(user.id==-1){
          break;
        }
        return user;
      }
    case 3:
      {
        clearScreen();
        printf("Exiting program");
        free(user.spot);
        exit(0);
      }
    default:
      printf("Enter a correct input number\n");
      //return GetAccount(pNumAccounts);
  }
  GetAccount(pNumAccounts);
}
int main(int argc, char const *argv[]) {
  Init();

  customer user;
  int numAccounts=0;
  user=GetAccount(&numAccounts);
  ManageSeats(user);
  return 0;
}
