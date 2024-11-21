# C Programming - Banking System 
Coursework 1 for Programming I (COMP-1312)\
Banking System by Mah Han Cheng

## Features
+ [Creating new bank accounts](#creating-new-bank-accounts)
+ [Deposit into / Withdraw from registered bank accounts](#deposit-into--withdraw-from-registered-bank-accounts)
+ [Remittance](#remittance)
+ [Delete bank accounts](#delete-bank-accounts)

## Creating new bank accounts
1. Creates account with:
    - **Name** _(3 to 32 characters)_
    - **Identification number** _(12-digit number)_
    - **Account number** _(Randomly generated between 10,000 to 1,000,000)_
    - **Account type** _(1 = Savings, 2 = Current Account)_
    - **Amount** _(Balance default RM 0.00)_

2. Creates a **'database' folder** to store accounts
3. Creates files in the 'database' folder, **storing account information** using the name **"account_number.txt"**.

## Deposit into / Withdraw from registered bank accounts
1. **Deposits to / Withdraws from** selected bank accounts
    - Checks if **amount to withdraw** is valid and not out of bounds (greater than 10 digits) to prevent overflow
    - Checks if **balance is enough to withdraw**
    - Checks if **account contents** are correct

## Delete bank accounts
1. **Delete** the specified bank account from the database _(Recovery is not avaliable)_

## Remittance
1. **Transfers** money from one account to another
2. **With remittance fee** deducted from the sender:
    - From **Current** Account to **Savings** Account (**2%** fee)
    - From **Savings** Account to **Current** Account (**3%** fee)
    - Checks if **amount to withdraw** is valid and not out of bounds (greater than 10 digits) to prevent overflow
    - Checks if **balance is enough to withdraw**
    - Checks if **account contents** are correct
      
## Code Language and Version
- C version: c17

## Setup
### To run this project (_Windows_):
1. Install a c compiler (mingw / winlibs) and add the binary directory to path.
2. To check if the compiler has installed:
In Command Prompt (cmd):
    ```sh
    gcc --version
    ```

3. In cmd / powershell, run:
    ```sh
    gcc banking_system_application.c -o banking_system_application      # To compile
    ```
4. To run:
    ```sh
    banking_system_application.exe                # in cmd
    ```
    ```sh
    ./banking_system_application.exe              # in powershell
    ```
4. The program should run perfectly now 🎉🎉🎉!!!

### To run this project (_Linux_):
1. GCC normally comes preinstalled, just run in terminal:
    ```sh
    gcc banking_system_application.c -o banking_system_application && ./banking_system_application
    ```

2. If GCC is not found / installed, run in terminal to install gcc:
    ```sh
    sudo apt install build-essential
    ```
3. and test install with:
    ```sh
    gcc --version
    ```

### To run this project (_MacOS_): 
1. Go to **App Store** and install **XCode**, Apple's compiler

2. Go to **Terminal** and run to install the command line tools of XCode:
    ```sh
    xcode-select --install
    ```

3. The setup should be done. To compile the file, run in **Terminal**:
    ```sh
    gcc -Wall -o banking_system_application banking_system_application.c    # compile
    ``` 

4. On newer versions of OS X, you can choose to compile with:
    ```sh
    clang banking_system_application.c -o banking_system_application        # compile
    ``` 

5. Then run the executable with:
    ```sh
    ./banking_system_application    # run program
    ```

## Author
- [Mah Han Cheng (36271632)](mailto:hcm1e24@soton.ac.uk)

## References
- [Compiling C on MacOS Based on code by Mark Setchell, Stack Overflow: https://stackoverflow.com/questions/32337643/how-can-i-run-a-c-program-on-mac-os-x-using-terminal](https://stackoverflow.com/questions/32337643/how-can-i-run-a-c-program-on-mac-os-x-using-terminal)
- [How to Install GCC Compiler on Linux? by anuragsingh1022, GeeksforGeeks: https://www.geeksforgeeks.org/how-to-install-gcc-compiler-on-linux/](https://www.geeksforgeeks.org/how-to-install-gcc-compiler-on-linux/)
- [C program to list all files in a directory recursively, Codeforwin: https://codeforwin.org/c-programming/c-program-to-list-all-files-in-a-directory-recursively](https://codeforwin.org/c-programming/c-program-to-list-all-files-in-a-directory-recursively)
