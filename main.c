#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/wait.h>

#define COLOR_RED "\033[31m"
#define COLOR_GOLD "\033[33m"
#define COLOR_CYAN "\033[38;2;36;255;255m" // #24ffff
#define COLOR_RESET "\033[0m"
#define MAX_CMD_LEN 2048

void print_banner() {
    printf(COLOR_GOLD);
    printf("░█████╗░██╗░░░░░░█████╗░██╗░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗\n");
    printf("██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗██╔════╝████╗░████║██╔══██╗██╔══██╗██╔════╝\n");
    printf("██║░░╚═╝██║░░░░░███████║██║░░░██║██║░░██║█████╗░░██╔████╔██║██║░░██║██║░░██║╚█████╗░\n");
    printf("██║░░██╗██║░░░░░██╔══██║██║░░░██║██║░░██║██╔══╝░░██║╚██╔╝██║██║░░██║██║░░██║░╚═══██╗\n");
    printf("╚█████╔╝███████╗██║░░██║╚██████╔╝██████╔╝███████╗██║░╚═╝░██║╚█████╔╝██████╔╝██████╔╝\n");
    printf("░╚════╝░╚══════╝╚═╝░░░░░░╚═════╝░╚═════╝░╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚════╝░╚════╝░\n");
    printf(COLOR_RESET);
    printf(COLOR_CYAN "claudemods arch Sysutil v1.0\n\n" COLOR_RESET);
}

void execute_command(const char *command) {
    printf(COLOR_CYAN "[EXECUTING] " COLOR_RESET);
    fflush(stdout);

    int status = system(command);

    if (status != 0) {
        printf(COLOR_RED "\n[ERROR] Command failed with status %d\n" COLOR_RESET, status);
    }
    else {
        printf(COLOR_CYAN "\n[SUCCESS] Command completed\n" COLOR_RESET);
    }

    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // Clear input buffer
}

int get_key() {
    int c = getchar();
    if (c == '\033') {
        getchar(); // Skip [
        return getchar(); // Return actual key code
    }
    return c;
}

int main() {
    char root_password[100];
    int selected = 0;
    const char *commands[] = {
        "Fix 'failed to synchronize all databases' for Pacman",
        "Fix 'unable to lock database' for Pacman",
        "Fix 'unable to lock database' for Pamac",
        "Fix clock time",
        "Fix connectivity issues",
        "Fix corrupted PGP signatures",
        "Fix corrupted packages",
        "Fix DNSCrypt issues",
        "Fix GPG key errors",
        "Fix login issues",
        "Fix login issues [NVIDIA]",
        "See system logs",
        "Update system",
        "Quit"
    };
    int num_commands = sizeof(commands) / sizeof(commands[0]);

    // Set terminal to raw mode
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    print_banner();

    // Get root password
    printf(COLOR_CYAN "Password: " COLOR_RESET);
    fflush(stdout);
    struct termios password_oldt, password_newt;
    tcgetattr(STDIN_FILENO, &password_oldt);
    password_newt = password_oldt;
    password_newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &password_newt);
    fgets(root_password, sizeof(root_password), stdin);
    tcsetattr(STDIN_FILENO, TCSANOW, &password_oldt);
    printf("\n");

    // Remove newline
    root_password[strcspn(root_password, "\n")] = 0;

    while (1) {
        system("clear");
        print_banner();

        printf(COLOR_CYAN "  Fix Arch Linux\n");
        printf("  --------------\n" COLOR_RESET);

        for (int i = 0; i < num_commands; i++) {
            if (i == selected) {
                printf(COLOR_GOLD "\033[1m➤ %s\033[0m\n\n" COLOR_RESET, commands[i]);
            } else {
                printf(COLOR_CYAN "  %s\n\n" COLOR_RESET, commands[i]);
            }
        }

        int c = get_key();

        switch (c) {
            case 'A': // Up arrow
                if (selected > 0) selected--;
                break;
            case 'B': // Down arrow
                if (selected < num_commands - 1) selected++;
                break;
            case '\n': {
                char command[MAX_CMD_LEN];
                strcpy(command, "echo '");
                strcat(command, root_password);
                strcat(command, "' | su -c '");

                if (strcmp(commands[selected], "Quit") == 0) {
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return 0;
                }
                else if (strcmp(commands[selected], "Fix 'failed to synchronize all databases' for Pacman") == 0) {
                    strcat(command, "rm -rf /var/lib/pacman/sync && rm -rf /var/tmp/pamac/dbs/sync && pacman -Sy'");
                }
                else if (strcmp(commands[selected], "Fix 'unable to lock database' for pacman") == 0) {
                    strcat(command, "rm /var/lib/pacman/db.lck'");
                }
                else if (strcmp(commands[selected], "Fix 'unable to lock database' for pamac") == 0) {
                    strcat(command, "rm /var/tmp/pamac/dbs/db.lck'");
                }
                else if (strcmp(commands[selected], "Fix clock time") == 0) {
                    strcat(command, "time_str=$(curl -sI \"http://google.com\" | grep -i \"^date:\" | cut -d\" \" -f2-) && date -s \"$time_str\" &>/dev/null && hwclock --systohc &>/dev/null'");
                }
                else if (strcmp(commands[selected], "Fix connectivity issues") == 0) {
                    strcat(command, "pacman -Rdd networkmanager --noconfirm && rm -rf /etc/NetworkManager && pacman -S networkmanager --noconfirm --needed && nmcli con add type ethernet ifname eth0 && reboot'");
                }
                else if (strcmp(commands[selected], "Fix corrupted PGP signatures") == 0) {
                    strcat(command, "pacman-key --refresh-keys'");
                }
                else if (strcmp(commands[selected], "Fix corrupted packages") == 0) {
                    strcat(command, "pacman -Qnq | xargs pacman -S --noconfirm --overwrite \"*\"'");
                }
                else if (strcmp(commands[selected], "Fix login issues") == 0) {
                    strcat(command, "update-grub && pacman -R --noconfirm linux linux-headers && pacman -S --noconfirm linux linux-headers'");
                }
                else if (strcmp(commands[selected], "Fix login issues [NVIDIA]") == 0) {
                    strcat(command, "pacman -R nvidia-dkms --noconfirm && pacman -S nvidia-dkms --noconfirm && pacman -R --noconfirm linux linux-headers && pacman -S --noconfirm linux linux-headers && update-grub'");
                }
                else if (strcmp(commands[selected], "Fix DNSCrypt issues") == 0) {
                    strcat(command, "pacman -Rdd dnscrypt-proxy --noconfirm && pacman -S dnscrypt-proxy --noconfirm && echo -e \"# Generated by NetworkManager\\nnameserver 127.0.0.1\" | tee /etc/resolv.conf'");
                }
                else if (strcmp(commands[selected], "Fix GPG key errors") == 0) {
                    strcat(command, "mv /etc/pacman.d/gnupg{,.bak} && pacman-key --init && pacman-key --populate archlinux && pacman-key --populate artix && pacman-key --recv-key FBA220DFC880C036 --keyserver keyserver.ubuntu.com && pacman-key --lsign-key FBA220DFC880C036 && pacman -U --noconfirm --needed 'https://cdn-mirror.chaotic.cx/chaotic-aur/chaotic-keyring.pkg.tar.zst' 'https://cdn-mirror.chaotic.cx/chaotic-aur/chaotic-mirrorlist.pkg.tar.zst' && pacman-key --populate manjaro'");
                }
                else if (strcmp(commands[selected], "See system logs") == 0) {
                    strcat(command, "dmesg'");
                }
                else if (strcmp(commands[selected], "Update system") == 0) {
                    strcat(command, "pacman -Syyu --noconfirm && update-grub'");
                }

                execute_command(command);
                break;
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}
