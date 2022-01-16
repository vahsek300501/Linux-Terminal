# KESHAV GAMBHIR
# 2019249

## Assignment-1 Documentation

### Internal Command:

    cd
        cd [directory]: To change the current directory to the specified directory
            [directory]: The path can be relative or absolute 
            Example: cd Desktop/folder  ->  relative
                     cd /home/keshav/Desktop/folder  -> absolute
        
        cd -P [directory]: To use the physical directory structure without symbolic links
        cd -L [directory]: To use the symbolic links instead of physical directory addresses

        cd .. : Takes back a directory
        cd ~ : Takes back to root directory

        cd / : Takes back to root directory
        cd : Takes back to root directory

        ERRORS HANDLED: IF THE DIRECTORY DOESN'T EXISTS [ERROR]
                        NESTED SYMBOLIC LINKS DIRECTORY [ATTACKS]

    echo
        echo [text]: Echos the text given in the square brackets in the next line.
        Example: echo "Hello World" -> Hello World

        echo -n [text]: removes the next line after the echo command.
        echo --help: prints the help menu on the shell

        ERRORS: INVALID ARGUMENTS
                "" ARE REMOVED
    

    history
        history: prints the history of the terminal

        history -c : clears all the history that existed
        history -w [file]: Writes the history to the file specified

        ERRORS HANDLED: IF THE HISTORY FILE IS NOT PRESENT
                        IF THE INPUT IS INVALID
                        IF THE FILE NAME IS NOT SPECIFIED AFTER -w
    
    pwd
        pwd: prints the current working directory

        pwd -P: Resolves the symlink to actual working directories
        pwd -L: Doesn't resolve the symlink to actual working directories

        ERROR HANDLED:INVALID INPUTS
                        ARGUMENTS MORE THAN REQuIRED
    
    exit
        exit: Exits the programm
        exit --help: prints the help menu of exit command
    
### External Commands

    ls
        ls: prints all the directories and files present in the current working directory
        ls [directory]: prints all the directories present in folder directory

        ls -a:print all the directories including directories starting with '.'
        ls -r:print all the directories in the reverse order of their names

        ERRORS HANDLED: FAILED TO OPEN DIRECTORY
                        INVALID DIRECTORY NAME
    
    cat
        cat [FILE]: concatenates files and print on the console
        cat: takes input from the console and prints it
        cat -E: To display $ at the end of each inputted lines
        cat -E [FILE]: to display $ at the end of each line inputted from the file.
        cat -n: To display the numbering of the lines
        cat -n [FILE]: To display numbering of the lines inputted from the file.

        ERRORS HANDLED: ERROR IN OPENING FILE
                        INVALID INPUT
    
    date
        date: prints the date in IST
        date -u: prints the date in UTC
        date -r [FILE]: prints the last modified date of the file.

        ERRORS HANDLED: INSUFEECIENT INPUTS
                        INVALID INPUTS
    
    mkdir
        mkdir [DIRECTORY]: To create the directory using both absolute and relative path
        mkdir -p [DIRECTORY]: makes directory accordingly ignoring if parent directory is present or not
                                if present: Don't create
                                else: Create
        mkdir -v [DIRECTORY]: prints the confirmation of the directory made

        ERRORS HANDLED: DIRECTORY ALREADY EXISTS
                        IF PATH IS INVALID
                        ERROR IN DIRECTORY CREATION
                        INVALID INPUTS
    
    rm
        rm [FILE]: removes the file present
        rm -i [FILE]: Confirmation before Removal of file
        rm -d [FILE]: Removes directories which are empty

        ERRORS:INVALID PATH
                NON EMPTY DIRECTORY
                NON EXISTANCE OF FILE
