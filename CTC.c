#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Struct Definitions
typedef struct
{
  char*firstname;
  char*lastname;
} NAME;

typedef struct
{
  int*streetno;
  char*streetname;
  char*streetname1;
  char*city;
  char*state;
  char*country;
  int*zip;
} ADDRESS;

typedef struct
{
  int*number;
} PHONE;

typedef struct
{
    NAME name;
    ADDRESS address;
    PHONE phone;
}PROFILE;

int countdata();
int displaymenu();
PROFILE*build(PROFILE*ptr,int count);
void populate(PROFILE*ptr,FILE*records,int count);
void searchoptions(PROFILE*ptr, int count);
void searchphone(PROFILE*ptr,int count);
void searchaddress(PROFILE*ptr,int count);
void street(PROFILE*ptr,int count);
void street1(PROFILE*ptr,int count);
void city(PROFILE*ptr,int count);
void state(PROFILE*ptr,int count);
void country(PROFILE*ptr,int count);
void zip(PROFILE*ptr,int count);
void house(PROFILE*ptr,int count);
void searchfullname(PROFILE*ptr,int count);
void deleteRecord(PROFILE*ptr,int *count,int *searchSaved);
void editrecord(PROFILE*ptr,int count);
void printdata(PROFILE*ptr,int count);

int main ()
{
	//Opens the file
    FILE*records;
    records=fopen("People.txt","r");
    if(!records)
        printf("Error opening the file");

    int searchSaved;
    int count;int option=0;
    
    //Counts how many records are in the file
    count=countdata();
	
	//Allocates the memory to build the database for all the information in the file
    PROFILE*pointer=NULL;
    pointer=build(pointer,count);
    if(pointer==NULL)
    {
        printf("Memory not correctly allocated");
        exit(1);
    }
	
	//Builds PROFILE struct. Has all the information in the file
    populate(pointer,records,count);

    //While loop allows multiple actions to take place in one run.
    while(option != 5 || option!=4)
    {
    	//Prompts user what they want to do
    	
        option=displaymenu();
        if(option==1)
            searchoptions(pointer,count);

        if(option==2)
        {
            searchoptions(pointer,count);
            deleteRecord(pointer,&count,&searchSaved);
        }

        if(option==3)
        {
            editrecord(pointer,count);
        }
        printf("\n");

        //Check against invalid input.
        if(option < 1 || option > 5)
            printf("I am sorry but %d was not one of the options on the menu.\nPlease pick the correct choice from the menu options!\n", option);

         if(option==4)
            printdata(pointer,count);
        if(option==5)
            exit(0);
    }

    return 0;
}

//Counts how many records in the file
int countdata()
{
    FILE*records;
    records=fopen("People.txt","r");
    if(!records)
        printf("Error opening the file");
    int scandata;int A=0;int streetnumber;
    
    //Loop runs until program reaches end of file
    for( ;scandata!=EOF; )
    {
        scandata=fscanf(records,"%*s%*s %d %*s %*s %*s %*s %*d",&streetnumber);
        if(scandata==1)
            A++;
    }
    
    //In case the file is empty
    if(A == 0)
    {
        printf("There are no records in the file. Program will terminate.");
        exit(0);
    }
    
    return A;
}

//Builds the PROFILE struct
PROFILE*build(PROFILE*ptr,int count)
{
    ptr=(PROFILE*)calloc(count,sizeof(PROFILE));
    if(ptr==NULL)
        printf("Memory not allocated.");
    return ptr;
}

//Gets all the data into the PROFILE struct
void populate(PROFILE*ptr,FILE*records,int count)
{
    int B;

    for(B=0;B<count;B++)
    {
        (*(ptr+B)).name.firstname=(char*)calloc(20,sizeof(char));
        (*(ptr+B)).name.lastname=(char*)calloc(20,sizeof(char));
        (*(ptr+B)).address.streetno=(int*)calloc(5,sizeof(int));
        (*(ptr+B)).address.streetname=(char*)calloc(20,sizeof(char));
        (*(ptr+B)).address.streetname1=(char*)calloc(20,sizeof(char));
        (*(ptr+B)).address.city=(char*)calloc(20,sizeof(char));
        (*(ptr+B)).address.state=(char*)calloc(20,sizeof(char));
        (*(ptr+B)).address.country=(char*)calloc(20,sizeof(char));
        (*(ptr+B)).address.zip=(int*)calloc(5,sizeof(int));
        (*(ptr+B)).phone.number=(int*)calloc(100,sizeof(int));

        fscanf(records,"%s %s",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
        fscanf(records,"%d %s %s %s %s %s %d",(ptr+B)->address.streetno,(ptr+B)->address.streetname,(ptr+B)->address.streetname1,
               (ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,(ptr+B)->address.zip);
        fscanf(records,"%d",(ptr+B)->phone.number);
    }
}

//Displays the options the user has
int displaymenu()
{
    int option;
    printf("            User Options\n");
    printf("-----------------------------------------\n");
    printf("1. Search by name, address or phone number\n");
    printf("2. Delete a record\n");
    printf("3. Edit an existing record\n");
    printf("4. Print all profiles\n");
    printf("5. Exit\n");
    printf("------------------------------------------\n");

    printf("Please select one of the above options: ");
    scanf("%d",&option);
    printf("\n");
    return option;
}

//Menu allows user to determine what part of record they want to search by
void searchoptions(PROFILE*ptr, int count)
{

    int option;

    printf("          Searh Options\n");
    printf("------------------------------------\n");
    printf("1. Phone Number\n");
    printf("2. Word in Address\n");
    printf("3. First and Last name\n");
    printf("4. Exit and quit program\n");
    printf("-------------------------------------\n");

    printf("Pick one of the above search options. ");
    scanf("%d",&option);
	
	//Searches for record by phone number
    if(option==1)
        searchphone(ptr,count);
    
    //Searches for record by address
    if(option==2)
        searchaddress(ptr,count);
    
    //Searches for record by name
    if(option==3)
        searchfullname(ptr,count);
        
    if(option==4)
        exit(0);

}

void searchfullname(PROFILE * ptr, int count)
{
    char*string;int matches=0;int*locarray;int B=0;int A = 0;int option;
    string=(char*)calloc(20,sizeof(char));

        printf("\n          Searh Options\n");
    printf("------------------------------------\n");
    printf("1. First name\n");
    printf("2. Last name\n");
    printf("-------------------------------------\n");
	
	//If user searches by name, user can determine if they want first or last name    
	printf("\nWhich part of the name do you want to search: ");
    scanf("%d", &option);

    printf("\n\nEnter the name string: ");
    scanf("%s",string);
	
	//Loop searches for location of matches in the database
    for(A=0;A<count;A++)
    {
        if(option == 1)
            if(strcmp(((*(ptr+A)).name.firstname),string) == 0)
                matches++;
        if(option == 2)
            if(strcmp(((*(ptr+A)).name.lastname),string) == 0)
                matches++;
    }

    if(matches==0)
        printf("No matches were found\n");
	
	//Creates an array that stores the locations for all the matches
    if(matches>0)
    {
        locarray=(int*)calloc(matches,sizeof(int));

        for(A=0;A<count;A++)
        {
            if(option == 1)
                if(strcmp(((*(ptr+A)).name.firstname),string) == 0)
                {
                    *(locarray+B)=A;
                    B++;
                }
            if(option == 2)
                if(strcmp(((*(ptr+A)).name.lastname),string) == 0)
                {
                    *(locarray+B)=A;
                    B++;
                }
        }

        printf("The profiles with all matches are now being printed.\n\n");
		
		//Prints all the matches that are found
        for(A=0;A<matches;A++)
        {
            B=*(locarray+A);
            printf("Name:%s %s\n",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+B)->address.streetno),(ptr+B)->address.streetname,(ptr+B)->address.streetname1);
            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));

            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,
                *((ptr+B)->address.zip));

            printf("Phone:%d\n\n",*((ptr+B)->phone.number));
        }
    }
}

//Searches for record using the phone number
void searchphone(PROFILE*ptr,int count)
{

    int number;int matches=0;int*locarray=NULL;int B=0;
    printf("Enter the phone number you are searching for. ");
    scanf("%d",&number);

    for(int A=0;A<count;A++)
    {
        if(*((ptr+A)->phone.number)==number)
            matches++;
    }

    if(matches==0)
        printf("No matches were found\n");

    if(matches>0)
    {
        locarray=(int*)calloc(matches,sizeof(int));
        for(int A=0;A<count;A++)
        {
            if(*((ptr+A)->phone.number)==number)
            {
                *(locarray+B)=A;
                B++;
            }
        }

        printf("The profiles with all matches are now being printed.\n\n");

        for(int A=0;A<matches;A++)
        {
            B=*(locarray+A);
            printf("Name:%s %s\n",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+B)->address.streetno),(ptr+B)->address.streetname,(ptr+B)->address.streetname1);
            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
            *((ptr+A)->address.zip));

            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,
            *((ptr+B)->address.zip));
            printf("Phone:%d\n\n",*((ptr+B)->phone.number));
        }
    }
}

//Searches for record using address
void searchaddress(PROFILE*ptr,int count)
{
    int opt;
	
	//Narrows down what part of address user wants to search by
    printf("What part of the address are you searching by? \n");
    printf("1. Name\n");
    printf("2. Type\n");
    printf("3. City\n");
    printf("4. State\n");
    printf("5. Country\n");
    printf("6. Zip\n");
    printf("7. House Number\n");
    scanf("%d",&opt);

    if(opt==1)
        street(ptr,count);
    if(opt==2)
        street1(ptr,count);
    if(opt==3)
        city(ptr,count);
    if(opt==4)
        state(ptr,count);
    if(opt==5)
        country(ptr,count);
    if(opt==6)
        zip(ptr,count);
    if(opt==7)
        house(ptr,count);
}

//Search for record using street name
void street(PROFILE*ptr,int count)
{
    char*string;int matches=0;int*locarray;int B=0;
    string=(char*)calloc(20,sizeof(char));
    printf("Enter the address string.");
    scanf("%s",string);

    for(int A=0;A<count;A++)
    {
        if(strcmp(((*(ptr+A)).address.streetname),string) == 0)
            matches++;
    }

    if(matches==0)
        printf("No matches were found\n");

    if(matches>0)
    {
        locarray=(int*)calloc(matches,sizeof(int));

        for(int A=0;A<count;A++)
        {
            if(strcmp(((*(ptr+A)).address.streetname),string) == 0)
            {
                *(locarray+B)=A;
                B++;
            }
        }

        printf("The profiles with all matches are now being printed.\n\n");
		
		//Prints all the records that match
        for(int A=0;A<matches;A++)
        {
            B=*(locarray+A);
            printf("Name:%s %s\n",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+B)->address.streetno),(ptr+B)->address.streetname,(ptr+B)->address.streetname1);
            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));

            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,
                *((ptr+B)->address.zip));

            printf("Phone:%d\n\n",*((ptr+B)->phone.number));
        }
    }
}

//Searches for record using street name
void street1(PROFILE*ptr,int count)
{
    char*string;int matches=0;int*locarray;int B=0;
    string=(char*)calloc(20,sizeof(char));
    printf("Enter the address string.");
    scanf("%s",string);

    for(int A=0;A<count;A++)
    {
        if(strcmp(((*(ptr+A)).address.streetname1),string) == 0)
            matches++;
    }

    if(matches==0)
        printf("No matches were found\n");

    if(matches>0)
    {
        locarray=(int*)calloc(matches,sizeof(int));

        for(int A=0;A<count;A++)
        {
            if(strcmp(((*(ptr+A)).address.streetname1),string) == 0)
            {
                *(locarray+B)=A;
                B++;
            }
        }

        printf("The profiles with all matches are now being printed.\n\n");

        for(int A=0;A<matches;A++)
        {
            B=*(locarray+A);
            printf("Name:%s %s\n",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+B)->address.streetno),(ptr+B)->address.streetname,(ptr+B)->address.streetname1);
            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));

            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,
                *((ptr+B)->address.zip));

            printf("Phone:%d\n\n",*((ptr+B)->phone.number));
        }
    }
}


//Searches for record by city
void city(PROFILE*ptr,int count)
{
    char*string;int matches=0;int*locarray;int B=0;
    string=(char*)calloc(20,sizeof(char));
    printf("Enter the address string.");
    scanf("%s",string);

    for(int A=0;A<count;A++)
    {
        if(strcmp(((*(ptr+A)).address.city),string) == 0)
            matches++;
    }

    if(matches==0)
        printf("No matches were found\n");

    if(matches>0)
    {
        locarray=(int*)calloc(matches,sizeof(int));

        for(int A=0;A<count;A++)
        {
            if(strcmp(((*(ptr+A)).address.city),string) == 0)
            {
                *(locarray+B)=A;
                B++;
            }
        }

        printf("The profiles with all matches are now being printed.\n\n");
		
		//Prints all the matches that are found
        for(int A=0;A<matches;A++)
        {
            B=*(locarray+A);
            printf("Name:%s %s\n",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+B)->address.streetno),(ptr+B)->address.streetname,(ptr+B)->address.streetname1);
            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));

            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,
                *((ptr+B)->address.zip));

            printf("Phone:%d\n\n",*((ptr+B)->phone.number));
        }
    }
}

//Searches for record by state
void state(PROFILE*ptr,int count)
{
    char*string;int matches=0;int*locarray;int B=0;
    string=(char*)calloc(20,sizeof(char));
    printf("Enter the address string.");
    scanf("%s",string);

    for(int A=0;A<count;A++)
    {
        if(strcmp(((*(ptr+A)).address.state),string) == 0)
            matches++;
    }

    if(matches==0)
        printf("No matches were found\n");

    if(matches>0)
    {
        locarray=(int*)calloc(matches,sizeof(int));

        for(int A=0;A<count;A++)
        {
            if(strcmp(((*(ptr+A)).address.state),string) == 0)
            {
                *(locarray+B)=A;
                B++;
            }
        }

        printf("The profiles with all matches are now being printed.\n\n");
		
		//Prints records that match state
        for(int A=0;A<matches;A++)
        {
            B=*(locarray+A);
            printf("Name:%s %s\n",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+B)->address.streetno),(ptr+B)->address.streetname,(ptr+B)->address.streetname1);
            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));

            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,
                *((ptr+B)->address.zip));

            printf("Phone:%d\n\n",*((ptr+B)->phone.number));
        }
    }
}

//Search for records by country
void country(PROFILE*ptr,int count)
{
    char*string;int matches=0;int*locarray;int B=0;
    string=(char*)calloc(20,sizeof(char));
    printf("Enter the address string.");
    scanf("%s",string);

    for(int A=0;A<count;A++)
    {
        if(strcmp(((*(ptr+A)).address.country),string) == 0)
            matches++;
    }

    if(matches==0)
        printf("No matches were found\n");

    if(matches>0)
    {
        locarray=(int*)calloc(matches,sizeof(int));

        for(int A=0;A<count;A++)
        {
            if(strcmp(((*(ptr+A)).address.country),string) == 0)
            {
                *(locarray+B)=A;
                B++;
            }
        }

        printf("The profiles with all matches are now being printed.\n\n");
		
		//Prints all the matches when search by country
        for(int A=0;A<matches;A++)
        {
            B=*(locarray+A);
            printf("Name:%s %s\n",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+B)->address.streetno),(ptr+B)->address.streetname,(ptr+B)->address.streetname1);
            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));

            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,
                *((ptr+B)->address.zip));

            printf("Phone:%d\n\n",*((ptr+B)->phone.number));
        }
    }
}

//Searches by ZIP code
void zip(PROFILE*ptr,int count)
{
    int string;int matches=0;int*locarray;int B=0;
    printf("Enter the zip code.");
    scanf("%d",&string);

    for(int A=0;A<count;A++)
    {
        if(*((ptr+A)->address.zip)==string)
            matches++;
    }

    if(matches==0)
        printf("No matches were found\n");
    
    
    if(matches>0)
    {
        locarray=(int*)calloc(matches,sizeof(int));

        for(int A=0;A<count;A++)
        {
            if(*((ptr+A)->address.zip)==string)
            {
                *(locarray+B)=A;
                B++;
            }
        }

        printf("The profiles with all matches are now being printed.\n\n");
        
		//Prints all the matches that are found
        for(int A=0;A<matches;A++)
        {
            B=*(locarray+A);
            printf("Name:%s %s\n",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+B)->address.streetno),(ptr+B)->address.streetname,(ptr+B)->address.streetname1);
            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));

            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,
                *((ptr+B)->address.zip));

            printf("Phone:%d\n\n",*((ptr+B)->phone.number));
        }
    }
}

//Searches for record using house number
void house(PROFILE*ptr,int count)
{
    int string;int matches=0;int*locarray;int B=0;
    printf("Enter the house number.");
    scanf("%d",&string);

    for(int A=0;A<count;A++)
    {
        if(*((ptr+A)->address.streetno)==string)
            matches++;
    }

    if(matches==0)
        printf("No matches were found\n");

    if(matches>0)
    {
        locarray=(int*)calloc(matches,sizeof(int));

        for(int A=0;A<count;A++)
        {
            if(*((ptr+A)->address.streetno)==string)
            {
                *(locarray+B)=A;
                B++;
            }
        }

        printf("The profiles with all matches are now being printed.\n\n");
		
		//Prints all the records that have the same house number
        for(int A=0;A<matches;A++)
        {
            B=*(locarray+A);
            printf("Name:%s %s\n",(ptr+B)->name.firstname,(ptr+B)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+B)->address.streetno),(ptr+B)->address.streetname,(ptr+B)->address.streetname1);
            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));

            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+B)->address.city,(ptr+B)->address.state,(ptr+B)->address.country,
                *((ptr+B)->address.zip));

            printf("Phone:%d\n\n",*((ptr+B)->phone.number));
        }
    }
}

//Allows user to delete record from a record from the database
void deleteRecord(PROFILE*ptr,int *count,int *searchFound)
{
    int D;

    if(*searchFound == -1)
        printf("Failed to delete a record\n");
    else
    {
        if((*count -1) == 1)
            *count -= 1;
        else
        {
            for(D = *searchFound; D < *count; D++)
            {
                *(ptr + D) = *(ptr + D + 1);
            }
            *count -= 1;
        }
        printf("\nFile %d deleted\n\n",(*searchFound+1));

    }
}

//Allows user to edit a record from the database
void editrecord(PROFILE*ptr,int count)
{
    if(count==0)
    {
        printf("No records are present to be modified.");
        exit(0);
    }

    int record;
    printf("Which record number would you like to change? ");
    scanf("%d",&record);
    printf("\n");
    printf("You will now be asked to change the records for this profile.\n\n");

    printf("Please enter the first and last name in that order\n");
    scanf("%s %s",(ptr+record-1)->name.firstname,(ptr+record-1)->name.lastname);

    printf("Now enter the streetnumber,street name,city,state,country and zip code\n");
    scanf("%d %s %s %s %s %s %d",(ptr+record-1)->address.streetno,(ptr+record-1)->address.streetname,(ptr+record-1)->address.streetname1,
               (ptr+record-1)->address.city,(ptr+record-1)->address.state,(ptr+record-1)->address.country,(ptr+record-1)->address.zip);

    printf("Now please enter the phone number\n");
    scanf("%d",(ptr+record-1)->phone.number);

    printf("The editing process is now complete.\n\n");
}

//Prints all the data if user wants to see all the records at once
void printdata(PROFILE*ptr,int count)
{
    int A;
    if(count == 0)
        printf("They are no records to be printed.\n\n");

    else
    {
        for(A=0;A<count-1;A++)
        {
            printf("Name:%s %s\n",(ptr+A)->name.firstname,(ptr+A)->name.lastname);
            printf("Address:%d %s %s\n",*((ptr+A)->address.streetno),(ptr+A)->address.streetname,(ptr+A)->address.streetname1);

            if(*((ptr+A)->address.zip)<10000)
                printf("City:%s   State:%s  Country: %s   Zip:%05d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));
            else
                printf("City:%s   State:%s  Country: %s   Zip:%d\n",(ptr+A)->address.city,(ptr+A)->address.state,(ptr+A)->address.country,
                *((ptr+A)->address.zip));

            printf("Phone:%d\n\n",*((ptr+A)->phone.number));
        }
    }
}

