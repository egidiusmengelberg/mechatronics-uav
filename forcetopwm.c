#include <stdio.h>
#include <conio.h>

// Define the structure for the lookup table entries
typedef struct {
    float force; // Force in Newtons
    int pwm;     // Corresponding PWM signal
} LookupTableEntry;

// Define the lookup tables for different modes
#define TABLE_SIZE 14
#define NUM_TABLES 6
// Define lookup table for motor 1 clockwise
//the first collum contains the force in Newtons and the second collum contains the corresponding PWM value (0-255)



LookupTableEntry lookupTable1[TABLE_SIZE] = {
    {0.0, 0},
    {0.018465882, 20},
    {0.036931765, 40},
    {0.050781176, 60},
    {0.064630588, 80},
    {0.073863529, 100},
    {0.087712941, 120},
    {0.092329412, 140},
    {0.101562353, 160},
    {0.125644706, 180},
    {0.108487059, 200},
    {0.113103529, 220},
    {0.120028235, 240},
    {0.131569412, 255}
   
};

//define lookup table for motor 1 counter clockwise 
//hereby table2 

LookupTableEntry lookupTable2[TABLE_SIZE] = {
    {0.0, 0},
    {0.02308235, 20},
    {0.050781176, 40},
    {0.076171765, 60},
    {0.092329412, 80},
    {0.108487059, 100},
    {0.120028235, 120},
    {0.136185882, 140},
    {0.143110588, 160},
    {0.152343529, 180},
    {0.161576471, 200},
    {0.163884706, 220},
    {0.170809412, 240},
    {0.177734118, 255}
};

//define lookup table for motor 2 clockwise
//hereby table3


LookupTableEntry lookupTable3[TABLE_SIZE] = {
    {0.0, 0},
    {0.020774118, 20},
    {0.03924, 40},
    {0.055397647, 60},
    {0.069247059, 80},
    {0.080788235, 100},
    {0.090021176, 120},
    {0.103870588, 140},
    {0.113103529, 160},
    {0.120028235, 180},
    {0.124644706, 200},
    {0.126952941, 220},
    {0.131569412, 240},
    {0.133877647, 255}
};

//define lookup table for motor 2 counter clockwise
//hereby table4



LookupTableEntry lookupTable4[TABLE_SIZE] = {
    {0.0, 0},
    {0.025390588, 20},
    {0.048472941, 40},
    {0.073863529, 60},
    {0.092329412, 80},
    {0.108487059, 100},
    {0.11772, 120},
    {0.133877647, 140},
    {0.145418824, 160},
    {0.147727059, 180},
    {0.150035294, 200},
    {0.15696, 220},
    {0.166192941, 240},
    {0.173117647, 255}
};

//define lookup table for motor 3 clockwise
//hereby table5

LookupTableEntry lookupTable5[TABLE_SIZE] = {
    {0.0, 0},
    {0.0073575, 20},
    {0.0220725, 40},
    {0.02943, 60},
    {0.0367875, 80},
    {0.044145, 100},
    {0.044145, 120},
    {0.05886, 140},
    {0.0662175, 160},
    {0.073575, 180},
    {0.0809325, 200},
    {0.08829, 220},
    {0.0956475, 240},
    {0.103005, 255}
};

//define lookup table for motor 3 counter clockwise
//hereby table6


LookupTableEntry lookupTable6[TABLE_SIZE] = {
    {0.0, 0},
    {0.0073575, 20},
    {0.014715, 40},
    {0.02943, 60},
    {0.044145, 80},
    {0.0515025, 100},
    {0.0662175, 120},
    {0.073575, 140},
    {0.0809325, 160},
    {0.0956475, 180},
    {0.1103625, 200},
    {0.1250775, 220},
    {0.132435, 240},
    {0.1397925, 255}
};

LookupTableEntry* lookupTables[NUM_TABLES] = {lookupTable1, lookupTable2, lookupTable3, lookupTable4, lookupTable5, lookupTable6};


//Create a function to interpolate between values if the exact force is not in the lookup table.
int interpolate(float force, LookupTableEntry *table, int size) {
    if (force <= table[0].force) {
        //print case 1
        printf("case 1 \n");

        return table[0].pwm;
    }
    if (force >= table[size - 1].force) {
        //print case 2
        printf("case 2 \n");
        

        return table[size - 1].pwm;
    }

    for (int i = 0; i < size - 1; i++) {
        if (force >= table[i].force && force <= table[i + 1].force) {
            float fraction = (force - table[i].force) / (table[i + 1].force - table[i].force);
            //print values chosen for interpolation
            //printf("Interpolating between %.2f N and %.2f N\n", table[i].force, table[i + 1].force);

            return table[i].pwm + fraction * (table[i + 1].pwm - table[i].pwm);
        }
    }

    return 0; // This should never be reached
}
int getPWMForForce(float desiredForce, int mode) {
    if (mode < 0 || mode >= NUM_TABLES) {
        printf("Invalid mode selected.\n");
        return -1;
    }
    return interpolate(desiredForce, lookupTables[mode], TABLE_SIZE);
}

int main() {
    float desiredForce; // force in Newtons
    int mode; // mode


    //print the first table for test
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("%.2f N -> %d\n", lookupTable1[i].force, lookupTable1[i].pwm);
    }


    //request desired force and mode from user
    printf("Enter the desired force in Newtons: ");
    scanf("%f", &desiredForce);
    printf("Enter the mode (0-5): ");
    scanf("%d", &mode);


    int pwmSignal = getPWMForForce(desiredForce, mode);
    
    if (pwmSignal != -1) {
        printf("PWM signal for force %.2f N in mode %d is %d\n", desiredForce, mode, pwmSignal);
    }

    printf("Press any key to exit.\n");
    getch();


    return 0;
}