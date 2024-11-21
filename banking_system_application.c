#include "helper.h"

int create_new_bank_account() {
    // name word limit
    int min_name_len = 3;
    int max_name_len = 32;

    char name[max_name_len + 2]; // +1 for null, +1 for extra
    
    // id digit limit
    int id_len = 12;
    char id[id_len + 2]; // +1 for null, +1 for extra

    char type_buffer[RESPONSE_BUFFER]; // temp buffer
    unsigned int account_type = 0;

    srand(time(NULL));

    while (1)
    {
        // Reset
        int rerun = 0;
        // Header
        header_ezprint("Create Bank Account");

        // Get Name
        printf("Enter Name [q to quit]: ");
        if (fgets(name, sizeof(name), stdin) == NULL) { // Name gotten
            printf("Error reading name.%s\n", TRYAGAIN);
            continue;
        }
        conditional_fgets_buffer_clearing(name);
        strip(name);

        // Quit
        if (strcmp(name, "q") == 0) return 0;

        if (strlen(name) > max_name_len || strlen(name) < min_name_len) {
            printf("Name length should be more than %d and less than %d.%s\n", min_name_len, max_name_len, TRYAGAIN);
            continue;
        } else
        {
            for (int i = 0; i < strlen(name); i++) {
                if (!isname(name)) 
                {
                    printf("Name should consist of only alphabets and no consecutive spaces.%s\n", TRYAGAIN);
                    rerun = 1;
                    break;
                }
            }
        }
        if (rerun) continue;


        // Get ID
        printf("\nEnter Identification Number (%d-digit number): ", id_len);
        if (fgets(id, sizeof(id), stdin) == NULL) { // ID gotten
            printf("Error reading id.%s\n", TRYAGAIN);
            continue;
        }
        conditional_fgets_buffer_clearing(id);
        strip(id);
        
        if (strlen(id) != id_len) {
            printf("Identification Number should be a %d-digit number.%s\n", id_len, TRYAGAIN);
            continue;

        } else {
            for (int i = 0; i < strlen(id); i++) {
                if (!isdigit(id[i])) {
                    printf("Identification Number should be a %d-digit number.%s\n", id_len, TRYAGAIN);
                    rerun = 1;
                    break;
                }
            }
        }
        if (rerun) continue;


        // Account type
        printf("\nEnter Account Type:\n");
        printf("1. Savings\n");
        printf("2. Current\n");
        printf("Select Option: ");

        if (fgets(type_buffer, sizeof(type_buffer), stdin) == NULL) {
            printf("Error reading input.%s\n", TRYAGAIN);
            continue;
        } 

        conditional_fgets_buffer_clearing(type_buffer);
        strip(type_buffer);

        if ((strcmp(type_buffer, "1") != 0 && strcmp(type_buffer, "2") != 0)) {
            printf("Input should be in the choices.%s\n\n", TRYAGAIN);
            continue;
        } 
        sscanf(type_buffer, "%d", &account_type); // Account type gotten

        break; // All done
    }

    // Creating directory and write file
    char complete_path[PATH_MAX];
    FILE *fptr;

    if (mkdir(DIR_NAME) == 0) {
        printf("\n\nLog: Creating '%s' folder.\n", DIR_NAME);
        printf("Log: Path '%s' created successfully.\n\n", DIR_NAME);
    } else {        
        if (errno == EEXIST) {
            printf("\n\nLog: %s already created.\n\n", DIR_NAME);
        } else {
            printf("\n\nLog: Failed to create '%s' folder. Error: %s\n\n", DIR_NAME, strerror(errno));
            return -1;
        }
    }

    // Get random account number (file name)
    int rand_account_number;

    // Prevent database overflow (exceed file limit)
    if (count_files_in_dir(DIR_NAME) >= ACCOUNT_RAND_RANGE_HIGH - ACCOUNT_RAND_RANGE_LOW + 1) {
        printf("\n\nLog: File limit reached.\n\n");
        return -1;
    }

    // generate random number for file name (account number), while loop to always get different file names
    do {
        rand_account_number = rand() % (ACCOUNT_RAND_RANGE_HIGH - ACCOUNT_RAND_RANGE_LOW + 1) + ACCOUNT_RAND_RANGE_LOW;
        snprintf(complete_path, sizeof(complete_path), "%s/%d.txt", DIR_NAME, rand_account_number);
        fptr = fopen(complete_path, "r");
        if (fptr != NULL) fclose(fptr);

    } while (fptr != NULL);
    fclose(fptr);

    // write choices and contents to new file (amount balance 0.00)
    fptr = fopen(complete_path, "w");
    char content[BUFSIZ];
    snprintf(content, sizeof(content), "%s\n%s\n%d\n%d\n0.00", name, id, rand_account_number, account_type);
    
    if (fprintf(fptr, "%s", content) < 0) {
        perror("\n\nError writing to file\n\n");
        fclose(fptr);
        return -1;
    }
    fclose(fptr);

    // Summary
    header_ezprint("Account Successfully Created");
    printf("Name: %s", name);
    printf("\nIdentification Number: %s", id);
    printf("\nAccount Number: %d", rand_account_number);
    printf("\nAccount type: %s", account_type == 1 ? "Savings" : "Current");
    printf("\nAmount: RM 0.00");

    return 0;
}

int delete_bank_account(int file_edited) {
    // make sure got files inside directory
    int file_count = count_files_in_dir(DIR_NAME);
    if (file_count < 1) {
        printf("There are no bank accounts registered in %s directory.", DIR_NAME);
        return -1;
    };

    // get all file paths
    char path_array[file_count][count_digits_with_base(ACCOUNT_RAND_RANGE_HIGH, 10) + 1];
    path_array_init(path_array, file_count, DIR_NAME);

    char buffer[RESPONSE_BUFFER];
    int account_to_delete;
    int rerun;

    while (1)
    {
        rerun = 0;
        // Options
        header_ezprint("Delete bank account");
        print_options_header("delete", file_count);
        for (int i = 0; i < file_count; i++)
        {
            printf("%d. %s\n", i+1, path_array[i]);
        }
        printf("\n");
        print_dashes(72);
        printf("\nSelect option: ");

        // Get Option to delete
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input.%s\n", TRYAGAIN);
            continue;
        } 

        conditional_fgets_buffer_clearing(buffer);
        strip(buffer);
        if (strcmp(buffer, "q") == 0) return 0;


        for (int i = 0; i < strlen(buffer); i++)
        {
            if (!isdigit(buffer[i])) {
                printf("Input should be in the choices.%s\n", TRYAGAIN);
                rerun = 1;
                break;
            }
        }
        if (rerun) continue;
        
        // input validation
        sscanf(buffer, "%d", &account_to_delete);
        if (account_to_delete > file_count || account_to_delete < 1 || buffer[0] == '\0') {
            printf("Input should be in the choices.%s\n", TRYAGAIN);
            continue;
        }

        // delete path and check if delete success
        char del_path[PATH_MAX];
        snprintf(del_path, sizeof(del_path), "%s/%s.txt", DIR_NAME, path_array[account_to_delete - 1]);

        if (remove(del_path) == 0) {
            printf("\n\nLog: Account '%s' successfully deleted.", path_array[account_to_delete - 1]);
        }
        else {
            printf("\n\nLog: Error deleting file '%s': %s (errno: %d)", del_path, strerror(errno), errno);
            return -1;
        }
        return 0;
    }
}

int perform_balance_change(char operand) {
    char tempfile[PATH_MAX] = "\0";
    snprintf(tempfile, sizeof(tempfile), "%s/%s.txt", DIR_NAME, TEMPFILE_NAME);

    // make sure got files inside directory
    int file_count = count_files_in_dir(DIR_NAME);
    if (file_count < 1) {
        printf("There are no bank accounts registered in %s directory.", DIR_NAME);
        return -1;
    };

    char path_array[file_count][count_digits_with_base(ACCOUNT_RAND_RANGE_HIGH, 10) + 1];
    path_array_init(path_array, file_count, DIR_NAME);

    char buffer[BUFSIZ];
    int acc_to_deposit = 0;
    double amount_to_change;

    char full_path[PATH_MAX] = "\0"; 
    FILE *file;

    int rerun;
    while (1)
    {
        rerun = 0;
        // Option chosen by user to deposit or withdraw
        if (operand == '+')
        {
            header_ezprint("Deposit");
            print_options_header("deposit", file_count);
        } else if (operand == '-')
        {
            header_ezprint("Withdrawal");
            print_options_header("withdraw", file_count);
            
        } else
        {
            printf("Error: Invalid operand. Did you play with the code?");
            exit(EXIT_FAILURE);
        }
        
        for (int i = 0; i < file_count; i++)
        {
            printf("%d. %s\n", i+1, path_array[i]);
        }

        // Choose account
        print_dashes(72);
        printf("\nSelect option: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input.%s\n", TRYAGAIN);
            continue;
        } 

        conditional_fgets_buffer_clearing(buffer);
        strip(buffer);
        if (strcmp(buffer, "q") == 0) return 0;
 
        for (int i = 0; i < strlen(buffer); i++)
        {
            if (!isdigit(buffer[i])) {
                printf("Input should be in the choices.%s\n", TRYAGAIN);
                rerun = 1;
                break;
            }
        }
        if (rerun) continue;
        
        sscanf(buffer, "%d", &acc_to_deposit);


        if (acc_to_deposit > file_count || acc_to_deposit < 1 || buffer[0] == '\0') {
            printf("Input should be in the choices.%s\n", TRYAGAIN);
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s.txt", DIR_NAME, path_array[acc_to_deposit - 1]);
        file = fopen(full_path, "r");  

        if (file == NULL) {
            printf("\n\nLog: Error opening file");
            return 1;
        }

        // get balance of chosen account
        char lines[LINES_IN_FILE][BUFSIZ];
        for (int _ = 0; _ < LINES_IN_FILE; _++) lines[_][0] = '\0';
        
        double balance;
        
        int i = 0;
        while (fgets(lines[i], sizeof(lines[0]), file)) {
            if (i == 4) {  
                if (!isfloat(lines[i])) {
                    printf("\nCould not grab balance. Check the %s.", DIR_NAME);
                    exit(EXIT_FAILURE);     
                };
                sscanf(lines[i], "%lf", &balance);  
                if (balance == 0 && operand == '-') {
                    printf("Current Balance: RM 0.00, Insufficient balance to withdraw.\n");
                    rerun = 1;
                }
                break;
            }
            i++;
        }
        fclose(file);
        if (rerun) continue;
        
        if (check_file(full_path) == 0) {
            printf("Invalid file contents, check the %s.", DIR_NAME);
            exit(EXIT_FAILURE);
        }

        printf("Current Balance: RM %s", lines[4]);

        // get amount to deposit
        if (operand == '+') printf("\nEnter amount to deposit [q to return to menu]: RM ");
        else printf("\n\nEnter amount to withdraw [q to return to menu]: RM ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input.%s\n", TRYAGAIN);
            continue;
        } 

        conditional_fgets_buffer_clearing(buffer);
        strip(buffer);
        if (strcmp(buffer, "q") == 0) continue;

        if (!isfloat(buffer) || tooManyFPoints(buffer)) {
            printf("Input should be a positive number with at most 2 decimal places.%s\n", TRYAGAIN);
            continue;
        }
        sscanf(buffer, "%lf", &amount_to_change);
        if (amount_to_change > LONG_MAX) {
                printf("\n\nLog: Amount limit reached. Please type a smaller number.\n\n");
                continue;
        }

        if (amount_to_change == 0) {
            printf("Input should not be 0.%s\n", TRYAGAIN);
            continue;
        }
           
        // Write to file
        if (operand == '+') balance += amount_to_change;
        else if (operand == '-')
        {
            if (balance >= amount_to_change && balance != 0) balance -= amount_to_change;
            else
            {
                printf("Insufficient balance. Current balance: %.2f", balance);
                continue;
            }
            
        }
        
        // in case of problem writing to file, recover past files from (tempfile) and revert file history
        rename(full_path, tempfile);
        file = fopen(full_path, "w");
        if (file == NULL) {
            printf("\n\nLog: Error writing to file. Recovering history records..., %s\n\n", TRYAGAIN);
            remove(full_path);
            rename(tempfile, full_path);
            return 1;
        }
        snprintf(lines[4], sizeof(lines[4]), "%.2f", balance);

        for (int _ = 0; _ < LINES_IN_FILE; _++)
        {
            if (fprintf(file, "%s", lines[_]) < 0) {
                perror("\n\nError writing to file\n\n");
                fclose(file);
                remove(full_path);
                rename(tempfile, full_path);
                return -1;
            }
        }
        
        // Success message and details 
        printf("\n\n");
        header_ezprint("Account Balance Successfully Changed");
        printf("Name: %s", lines[0]);
        printf("Identification Number: %s", lines[1]);
        printf("Account Number: %s", lines[2]);
        printf("Account type: %s\n", lines[3][0] == '1' ? "Savings" : "Current");
        printf("Amount: RM %s", lines[4]);
    
        fclose(file);
        remove(tempfile);
        return 0;
    }
}

int remittance() {
    // make sure got files inside directory
    int file_count = count_files_in_dir(DIR_NAME);
    if (file_count < 2) {
        printf("There are less than 2 bank accounts registered.");
        return -1;
    };

    char tempfile[2][PATH_MAX] = {"\0", "\0"};
    for (int _ = 0; _ < 2; _++) snprintf(tempfile[_], sizeof(tempfile)[_], "%s/%s_%d.txt", DIR_NAME, TEMPFILE_NAME, _);
    
    // get all file paths in directory
    char path_array[file_count][count_digits_with_base(ACCOUNT_RAND_RANGE_HIGH, 10) + 1];
    path_array_init(path_array, file_count, DIR_NAME);

    char buffer[BUFSIZ];
    
    char path_buffer[2][PATH_MAX] = {"\0", "\0"};
    int f = 0;

    int sender_acc_type, receiver_acc_type;
    double transfer_amount;
    double remittance_fee;
    double taxed_transfer_amount;

    FILE *file;

    char sender_lines[LINES_IN_FILE][BUFSIZ];
    char receiver_lines[LINES_IN_FILE][BUFSIZ];
    for (int _ = 0; _ < LINES_IN_FILE; _++) {
        sender_lines[_][0] = '\0';
        receiver_lines[_][0] = '\0';
    }

    double balance;
    int rerun = 0;
    while (1)
    {
        f = 0;
        int i = 0;
        
        // Choose accounts to transfer from and to
        while (i < 2)
        {
            rerun = 0;
            if (i == 0) {
                header_ezprint("Remittance");
                printf("Which account to transfer from? [q to quit]");
            }
            else printf("Which account to transfer to? [q to quit]");
            printf("\n");

            for (int _ = 0; _ < file_count; _++)
            {
                printf("%d. %s\n", _+1, path_array[_]);
            }
            print_dashes(72);
            printf("\nSelect option: ");
            

            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                printf("Error reading input.%s\n", TRYAGAIN);
                continue;
            } 
            conditional_fgets_buffer_clearing(buffer);
            strip(buffer);
            if (strcmp(buffer, "q") == 0) return 0;
    
            for (int _ = 0; _ < strlen(buffer); _++)
            {
                if (!isdigit(buffer[_])) {
                    printf("Input should be in the choices.%s\n", TRYAGAIN);
                    rerun = 1;
                    break;
                }
            }
            if (rerun) continue;
            
            sscanf(buffer, "%d", &f);
            
            if (f > file_count || f < 1 || buffer[0] == '\0') {
                printf("Input should be in the choices.%s\n\n", TRYAGAIN);
                continue;
            }
            strcpy(path_buffer[i], path_array[f - 1]);
            // check if sender and receiver are the same
            if (strcmp(path_buffer[0], path_buffer[1]) == 0) {
                printf("Both the transferor and receipient cannot be the same.%s\n", TRYAGAIN);
                i = 0;
                continue;
            }
            i++;
            printf("\n");
        }

        // Get transfer_amount
        char temp[PATH_MAX];
        strcpy(temp, path_buffer[0]);
        snprintf(path_buffer[0], sizeof(path_buffer[0]), "%s/%s.txt", DIR_NAME, temp);
        strcpy(temp, path_buffer[1]);
        snprintf(path_buffer[1], sizeof(path_buffer[1]), "%s/%s.txt", DIR_NAME, temp);

        if (check_file(path_buffer[0]) == 0) {
            printf("Invalid file contents, check the %s.", DIR_NAME);
            exit(EXIT_FAILURE);
        }

        file = fopen(path_buffer[0], "r");
        if (file == NULL) {
            printf("\n\nLog: Error opening file");
            return 1;
        }

        // grab current balance of sender and receiver      
        i = 0;
        while (fgets(sender_lines[i], sizeof(sender_lines[0]), file)) {
            if (i == 4) {  
                if (!isfloat(sender_lines[i])) {
                    printf("\nCould not grab balance. Check the %s.", DIR_NAME);
                    exit(EXIT_FAILURE);     
                };
                sscanf(sender_lines[i], "%lf", &balance);  
                break;
            }
            i++;
        }
        fclose(file);
                
        if (check_file(path_buffer[1]) == 0) {
            printf("Invalid file contents, check the %s.", DIR_NAME);
            exit(EXIT_FAILURE);
        }

        file = fopen(path_buffer[1], "r");
        i = 0;
        while (fgets(receiver_lines[i], sizeof(receiver_lines[0]), file)) {
            if (i == 4) {  
                if (!isfloat(receiver_lines[i])) {
                    printf("\nCould not grab balance. Check the %s.", DIR_NAME);
                    exit(EXIT_FAILURE);     
                };
                break;
            }
            i++;
        }
        fclose(file);

        // grab both accounts' file type
        sender_acc_type = sender_lines[3][0];
        receiver_acc_type = receiver_lines[3][0];

        if (sender_lines[3][1] != '\n' || (sender_acc_type != '1' && sender_acc_type != '2')) {
            strip(sender_lines[2]);
            printf("Abnormallies detected in file %s, check your %s.", sender_lines[2], DIR_NAME);
            exit(EXIT_FAILURE);

        } else if (receiver_lines[3][1] != '\n' || (receiver_acc_type != '1' && receiver_acc_type != '2')) {
            strip(receiver_lines[2]);
            printf("Abnormallies detected in file %s, check your %s.", receiver_lines[2], DIR_NAME);
            exit(EXIT_FAILURE);
        }
        
        // 1 = Savings, 2 = Current, 1 -> 2 2%, 2 -> 1 3%
        if (sender_acc_type != receiver_acc_type) 
        {   
            if (sender_acc_type == '1') remittance_fee = 1.02;
            else if (receiver_acc_type == '1') remittance_fee = 1.03;
            else
            {
                printf("Abnormallies detected. Check your %s", DIR_NAME);
                exit(EXIT_FAILURE);
            }
            
        } else remittance_fee = 1;
        

        while (1)
        {
            rerun = 0;
            printf("Current Balance: RM %.2f", balance);
            if (balance == 0) {
                printf("\nInsufficient balance.");
                rerun = 1;
                break;
            }
            // Amount to transfer and check if valid
            printf("\nEnter amount to transfer");

            if (remittance_fee == 1.02) printf(" (Savings to Current remittance fee = 2%%)");
            else if (remittance_fee == 1.03) printf(" (Current to Savings remittance fee = 3%%)");

            printf(" [q to return to choose account]: RM ");

            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                printf("Error reading input.%s\n", TRYAGAIN);
                continue;
            } 
            conditional_fgets_buffer_clearing(buffer);
            strip(buffer);
            if (strcmp(buffer, "q") == 0) {
                rerun = 1;
                break;
            }
            if (!isfloat(buffer) || tooManyFPoints(buffer)) {
                printf("Input should be a positive number with maximum 2 decimal places.%s\n\n", TRYAGAIN);
                continue;
            }
            sscanf(buffer, "%lf", &transfer_amount);
            if (transfer_amount == 0) {
                printf("Input should not be 0.%s\n\n", TRYAGAIN);
                continue;
            }
            
            // calculate taxed transfer amount and check if exceed current sender balance
            taxed_transfer_amount = double_floor(transfer_amount * remittance_fee, 2);
            if (taxed_transfer_amount < 0 || taxed_transfer_amount > LONG_MAX) {
                printf("\n\nLog: Amount limit reached. Please type a smaller number.\n\n");
                continue;
            }
            while (1)
            {
                // provide total payment needed with remittance fee and prompt confirmation
                buffer[0] = '\0';
                printf("Required amount: RM %.2f. Are you sure? (Y/N): ", taxed_transfer_amount);

                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    printf("Error reading input.%s\n", TRYAGAIN);
                    continue;
                } 
                conditional_fgets_buffer_clearing(buffer);
                strip(buffer);
                
                if (strcmp(buffer, "Y") == 0 || strcmp(buffer, "y") == 0) {
                    // pass
                } else if (strcmp(buffer, "N") == 0 || strcmp(buffer, "n") == 0) {
                    rerun = 1;
                } else {
                    printf("Invalid input, %s\n", TRYAGAIN);
                    continue; 
                }
                printf("\n", TRYAGAIN);
                break;
            }
            if (rerun) continue;

            if (balance < taxed_transfer_amount) {
                printf("\nInsufficient balance. Required amount: RM %.2f\n\n", taxed_transfer_amount);
                continue;
            }

            snprintf(sender_lines[4], sizeof(sender_lines[4]), "%.2f", balance - taxed_transfer_amount);

            // write to sender's file '-'
            rename(path_buffer[0], tempfile[0]);
            file = fopen(path_buffer[0], "w");
            for (int x = 0; x < LINES_IN_FILE; x++)
            {
                if (fprintf(file, "%s", sender_lines[x]) < 0) {
                    printf("\n\nLog: Error writing to file. Recovering history records..., %s", TRYAGAIN);
                    fclose(file);

                    for (int _ = 0; _ <= i; _++)
                    {
                        remove(path_buffer[0]);
                        rename(tempfile[0], path_buffer[0]);
                    }
                    return -1;
                }
            }
            fclose(file);

            // second file
            sscanf(receiver_lines[4], "%lf", &balance);
            snprintf(receiver_lines[4], sizeof(receiver_lines[4]), "%.2f", balance += transfer_amount);
            rename(path_buffer[1], tempfile[1]);
            file = fopen(path_buffer[1], "w");
            for (int x = 0; x < LINES_IN_FILE; x++)
            {
                if (fprintf(file, "%s", receiver_lines[x]) < 0) {
                    printf("\n\nLog: Error writing to file. Recovering history records..., %s\n\n", TRYAGAIN);
                    fclose(file);

                    for (int _ = 0; _ < 2; _++)
                    {
                        remove(path_buffer[_]);
                        rename(tempfile[_], path_buffer[_]);
                    }
                    return -1;
                }
            }
            remove(tempfile[0]);
            remove(tempfile[1]);
            fclose(file);

            // summary from sender file (no need for receiver file because sender doesn't need to see receiver's balance)
            printf("\n\n");
            header_ezprint("Amount successfully transferred.");

            printf("Name: %s", sender_lines[0]);
            printf("Identification Number: %s", sender_lines[1]);
            printf("Account Number: %s", sender_lines[2]);
            printf("Account type: %s\n", sender_lines[3][0] == '1' ? "Savings" : "Current");
            printf("Amount: RM %s", sender_lines[4]);
            break;
        }
        if (rerun) continue;
        return 0;
    }
}  

int main() {
    int file_edited = 0;
    while (1) {
        char response_buffer[RESPONSE_BUFFER]; // temporary buffer
        int response = 0;

        // header
        main_menu();
        
        // Account type
        if (fgets(response_buffer, sizeof(response_buffer), stdin) == NULL) {
            printf("Error reading input.%s\n", TRYAGAIN);
            continue;
        } 

        conditional_fgets_buffer_clearing(response_buffer);
        strip(response_buffer);

        if (strcmp(response_buffer, "q") == 0) return 0;

        if (strlen(response_buffer) != 1) {
            printf("Input should be a number (1 - 5).%s\n", TRYAGAIN);
            continue;
        } 
        sscanf(response_buffer, "%d", &response); // response gotten


        switch (response)
        {
        case 1:
            create_new_bank_account();
            break;
        case 2:
            perform_balance_change('+');
            file_edited = 1;
            break;
        case 3:
            perform_balance_change('-');
            file_edited = 1;
            break;
        case 4:
            remittance();
            file_edited = 1;
            break;
        case 5:
            delete_bank_account(file_edited);
            break;
        default:
            printf("Please enter a valid number.");
            break;
        }
    }
}

