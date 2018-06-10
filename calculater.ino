#include <IRremote.h>
#include <IRremoteInt.h>
#include <LiquidCrystal.h>

const int RECV_PIN = 9;    
short int data = 0;   
short int gloc = 0; 
char screen[30];
short int ifoperator[30];
byte minus[8] = {
	B00000,
	B00000,
	B00000,
	B01110,
	B00000,
	B00000,
	B00000,
};		
IRrecv irrecv(RECV_PIN);   
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 5, 4, 3, 2);
decode_results irresults;

double calculation(double num[], short int operator_typ[], short int operator_num){
	//change nifix expression to postfix expression
	short int total_length = (sizeof(num) / sizeof(num[0])) + operator_num;
	short int *stack = new short int[operator_num]{};
	short int *store = new short int[total_length]{};
	*store = 0;
	short int compare = -1;
	int location_store = 1;
	int location_stack = 0;
	for(short int i=0; i<operator_num; i++){	
		if(i == 0){
		   *(stack + location_stack) = operator_typ[i];
			compare = operator_typ[i];
			location_stack++; 
		}
		else{
			if(compare < 3 && operator_typ[i] > 3){
				*(stack + location_stack) = operator_typ[i];
				compare = operator_typ[i];
				location_stack++;	
			}
			else if(operator_typ[i] < 3){
				for(int j=location_stack-1; j>=0; j--){
					*(store + location_store + (location_stack -1 - j)) = *(stack + j); 
				}
				location_store = location_store + location_stack;
				location_stack = 0;
				*(stack + location_stack) = operator_typ[i];
				location_stack++;
			}
			else{
				while(*(stack + location_stack -1) > 3){
				*(store + location_store) = *(stack + location_stack -1); 
				location_store = location_store + 1; 
				location_stack --;
				}
				*(stack + location_stack) = operator_typ[i];
				compare = operator_typ[i];
				location_stack++;
			}
		}
		*(store + location_store) = 0;
		if(i == operator_num-1){
			for(int j=location_stack-1; j>=0; j--){
				*(store + location_store + (location_stack - j)) = *(stack + j); 
			}
			location_store = location_store + location_stack;
		}
		location_store++;
	}
	delete stack;
	char fin_store[29];
	for(int i=0; i<location_store; i++){ 
		fin_store[i] = char(*(store + i)); 
	}
	fin_store[location_store] = '\0';
	//start calculation
	double *num_stack = new double[operator_num]{};
	short int numstack_loc = 0;
	short int numloc = 0;
	for(int i=0; i<location_store; i++){
		switch (fin_store[i]) {
			case 0x00:
			*(num_stack + numstack_loc) = num[numloc];
			numloc++;
			numstack_loc++;
			break;
			case 0x01:
			*(num_stack + (numstack_loc - 2)) = *(num_stack + (numstack_loc - 2)) - *(num_stack + (numstack_loc - 1));
			numstack_loc--;
			break;
			case 0x02:
			*(num_stack + (numstack_loc - 2)) = *(num_stack + (numstack_loc - 2)) + *(num_stack + (numstack_loc - 1));
			numstack_loc--;
			break;
			case 0x04:
			*(num_stack + (numstack_loc - 2)) = *(num_stack + (numstack_loc - 2)) / *(num_stack + (numstack_loc - 1));
			numstack_loc--;
			break;
			case 0x05:
			*(num_stack + (numstack_loc - 2)) = *(num_stack + (numstack_loc - 2)) * *(num_stack + (numstack_loc - 1));
			numstack_loc--;
			break;
		}
	}
	double result = *(num_stack);
	return result;
	delete store;
	delete num_stack;
	store = NULL;
	stack = NULL;
	num_stack = NULL;
}

double str2double(char mat[], short int size){	//transfer str into double
	char *temp = new char[size]{};
	char *temp_p;
	temp_p = temp;
	for(short int i=0; i<size; i++){
		*(temp + i) = mat[i]; 
		if(i != 0 && mat[i] == '-'){
		  synError();
		  irrecv.resume();
		  irrecv.decode(&irresults);
		  while(irresults.value != 0xFFC23D && irresults.value != 0xFFA25D && irresults.value != 0xFFE21D){
			    delay(100);
			    irrecv.resume();
			    delay(100);
			    irrecv.decode(&irresults);
			}
			lcd.noBlink();
			lcd.noCursor();
			lcd.setCursor(0,0);
			gloc = 0;
		}
	}
	*(temp + size) = '\0';
	double dtemp = strtod (temp_p, NULL);
	delete [] (temp);
	temp = NULL;
	return dtemp;
}

void synError() {
	lcd.clear(); 
	lcd.noCursor();
	lcd.noBlink();
	lcd.home();
	lcd.print("Syntax ERROR");
	lcd.setCursor(0,1);
	lcd.print("Press AC or DIR");
}

void setup() {
	Serial.begin(9600);
  irrecv.enableIRIn(); // enable ir receiver  
  // set up the LCD's number of columns and rows: 
  lcd.createChar(5, minus);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear(); 
  lcd.cursor();
  lcd.blink();
  lcd.home();
}

void loop() {
	char ifequal = 0;
	char *screenchar;
	screenchar = screen;
	char input = 0;
	if(irrecv.decode(&irresults))   
	{
		switch (irresults.value) {
			case 0xFF52AD:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='9';
			input = 1;
			break;
			case 0xFF4AB5:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='8';
			input = 1;
			break;
			case 0xFF42BD:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='7';
			input = 1;
			break;
			case 0xFF5AA5:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='6';
			input = 1;
			break;
			case 0xFF38C7:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='5';
			input = 1;
			break;
			case 0xFF10EF:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='4';
			input = 1;
			break;
			case 0xFF7A85:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='3';
			input = 1;
			break;
			case 0xFF18E7:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='2';
			input = 1;
			break;
			case 0xFF30CF:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='1';
			input = 1;
			break;
			
			case 0xFFB04F:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='-';	
			input = 1;
			break;

			case 0xFF9867:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='.';
			input = 1;
			break;

			case 0xFF6897:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 0;
			screen[gloc]='0';
			input = 1;
			break;
			case 0xFF906F:
			ifequal = 1;
			break;
			case 0xFFA857:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 2;
			screen[gloc]='+';
			input = 1;
			break;
			case 0xFFE01F:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 1;
			screen[gloc]='-';
			input = 1;
			break;
			case 0xFFC23D:			//clear
			lcd.blink();
			lcd.cursor();
			screen[30] = {};
			for(short int i=0; i<30; i++){
				ifoperator[i] = 3; 
			}
			lcd.clear();
			lcd.home();
			gloc = 0;
			data = 0;
			break;
			case 0xFF02FD:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 5;
			screen[gloc]='*';
			input = 1;
			break;
			case 0xFF22DD:
			lcd.blink();
			lcd.cursor();
			ifoperator[gloc] = 4;
			screen[gloc]='/';
			input = 1;
			break;
			case 0xFFE21D:			// cursor move right
			lcd.blink();
			lcd.cursor();
			if(gloc == data){
				break;    
			}
			else{
				if(gloc >= 15){
					lcd.scrollDisplayLeft(); 
					lcd.setCursor(gloc+1,0);
					gloc = gloc + 1; 
				}
				else{
					lcd.setCursor(gloc+1,0);
					gloc = gloc + 1;
				}
				break;
			}

			case 0xFF629D:		//delete
			lcd.blink();
			lcd.cursor();
			if(gloc > 0){
				if(gloc == data){
					gloc = gloc - 1;
					data = data - 1;
					lcd.setCursor(gloc,0);
					lcd.write(byte(0x20));
					lcd.setCursor(gloc,0);
					if(gloc >= 15){
						lcd.scrollDisplayRight(); 
					}
				}
				else{
					for(int i = 0; i < data - gloc - 1; i++){
						screen[gloc + i] = *(screenchar + gloc + i + 1);
						ifoperator[gloc + i] = ifoperator[gloc + i + 1];
						if(*(screenchar + gloc + i) == '-' && ifoperator[gloc + i] == 0){
							lcd.write(byte(5));
						}
						else{
							lcd.print(*(screenchar + gloc + i )); 
						}
					}
					data =data - 1;
					lcd.write(byte(0x20));
					lcd.setCursor(gloc,0);
				}
			}
			break;
			case 0xFFA25D:			//cursor move left
			lcd.blink();
			lcd.cursor();
			if(gloc == 0){
				break;    
			}
			else{
				if(gloc > 15){
					lcd.scrollDisplayRight(); 
					lcd.setCursor(gloc-1,0);
					gloc = gloc - 1;
				}   
				else{
					lcd.setCursor(gloc-1,0);
					gloc = gloc - 1;
				}
				break;
			}
		}
		/* switch finish */
		if(data < 29){
			if(gloc == data){
				if(input == 1){
					if(*(screenchar + gloc) == '-' && ifoperator[gloc] == 0){
						lcd.write(byte(5));
					}
					else{
						lcd.print(*(screenchar + gloc));
					}
					gloc = gloc + 1;
					data = data + 1;
					if(gloc > 15){ 
						lcd.scrollDisplayLeft(); 
					}
				}
			}

			else{
				if(input == 1){
					//Serial.println(*(screenchar + gloc));
					if(*(screenchar + gloc) == '-' && ifoperator[gloc] == 0){
						lcd.write(byte(5));
					}
					else{
						lcd.print(*(screenchar + gloc));
					}
					gloc = gloc + 1;
					if(gloc > 15){ 
						lcd.scrollDisplayLeft(); 
					}
				}
			}
		}
		irrecv.resume();
	}
	if(ifequal == 1){						//equal button is pressed	
			if(gloc > 15){
				lcd.home();
			}	
			short int operator_num = 0;					//the number of operator
			for(short int i = 0; i < data; i++){
				if(ifoperator[i] != 0){
					operator_num++;				//obtain the number of operator
				}	
			}
			if(operator_num == 0){
				lcd.setCursor(0,1);
				lcd.print("=");
				char numtemp1[data];
					for(short int i=0; i<data; i++){
						numtemp1[i]= *(screenchar + i);
					}
					double num1 = str2double(numtemp1, data);
					if(num1 < 0){
					    num1 = -1.0 * num1;
					    lcd.write(byte(5));
					    lcd.print(num1);
					}
					else{
						lcd.print(num1);
					}
			}
			else{
				double num[operator_num + 1];
				short int *operator_loc = new short int[operator_num + 2]; //array to store the location of the operator
				short int *operator_typ = new short int[operator_num]; //array to store the type of the operator
				operator_loc[0] = -1;
				operator_loc[operator_num + 1] = data;
				//store the location and the type of operator
				short int count = 0;
				for(short int i = 0; i < data; i++){
					if(ifoperator[i] != 0){
						operator_loc[count + 1] = i;
						operator_typ[count] = ifoperator[i];
						count++;
					}	
				}
				//store the numbers between the operators
				short int noERROR;
				for(short int i=0; i<operator_num + 1; i++){
					short int temp = operator_loc[i + 1] - operator_loc[i] -1;
					noERROR = temp;
					if(temp == 0){
						synError();
						irrecv.resume();
						irrecv.decode(&irresults);
						delay(100);
						while(irresults.value != 0xFFC23D && irresults.value != 0xFFA25D && irresults.value != 0xFFE21D){
						    delay(100);
						    irrecv.resume();
						    delay(100);
						    irrecv.decode(&irresults);
						    Serial.println(1);
						}
						break;
					}
					else{
						char numtemp[temp];
						for(short int j=0; j<temp; j++){
							numtemp[j]= *(screenchar + operator_loc[i] + 1 + j) ;
						}
						num[i] = str2double(numtemp, temp);
					}

				}
				if(noERROR != 0){
					double result = calculation(num,operator_typ, operator_num);
					lcd.setCursor(0,1);
					lcd.print("=");
					if(result < 0){
					    result = -1.0 * result;
					    lcd.write(byte(5));
					    lcd.print(result);
					}
					else{
						lcd.print(result);
					}
				}
			}
			lcd.noBlink();
			lcd.noCursor();
			lcd.setCursor(0,0);
			gloc = 0;		
		}
	}

