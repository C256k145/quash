#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_LEN 100

std::string readLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

std::string* createCommand(std::string input, int &length) {
    int len = input.length();
    std::string *command = new std::string[MAX_LEN];
    int index = 0;
    char c;
    for(int i = 0; i < len; i++) {
        c = input[i];
        if(c == ' ')
            index++;
        else
            command[index] += c;
    }
    length = index + 1;
    return command;
}

void changedir(char** args, int len) {
    char* home = getenv("HOME");
    if(len == 1)
        chdir(home);
    else
        chdir(args[1]);
}

int execute(char** args, int len) {
    if(strcmp(args[0], "cd") == 0)
        changedir(args, len);
    else if(strcmp(args[0], "exit") == 0 || strcmp(args[0], "quit") == 0)
        exit(0);
    else if(strcmp(args[0], "jobs") == 0)
        exit(0);
    else {
        int pid = fork();
        if(pid == 0) {
            fflush(stdout);
            execvp(args[0], args);
        }
        else {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}

char** to_chars(std::string* temp, int &length) {
    char** res = new char*[length];
    for(int i = 0; i < length; i++) {
        res[i] = new char[temp[i].size()];
        strcpy(res[i], temp[i].c_str());
    }
    return res;
}

void mainLoop() {
    std::string input;
    char** args;
    std::string* temp;
    int res, len;

    char *buf;
    size_t size = 1024;

    while(true) {
        std::cout << getcwd(buf, size) << "$ ";
        input = readLine();
        temp = createCommand(input, len);
        args = to_chars(temp, len);
        res = execute(args, len);
    }

}

int main(int argc, char **argv, char **envp) {
    mainLoop();
    return 0;
}