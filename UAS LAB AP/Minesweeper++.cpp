#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Minesweeper {
private:
    int board[10][10]; // -1 untuk bom, 0-8 untuk jumlah bom di sekitar
    char view[10][10]; // '#' tertutup, 'F' flag, '0'-'8' terbuka, '*' bom
    int size; // ukuran papan
    int totalBombs; 
    int flagsPlaced;
    time_t startTime;
    bool gameOver;

    // Method untuk menghitung jumlah bom di 8 titik tetangga
    int countAdjacentBombs(int r, int c) {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int nr = r + i;
                int nc = c + j;
                if (nr >= 0 && nr < size && nc >= 0 && nc < size) {
                    if (board[nr][nc] == -1) count++;
                }
            }
        }
        return count;
    }

    // Implementasi rekursi untuk flood fill
    void floodFill(int r, int c) {
        if (r < 0 || r >= size || c < 0 || c >= size) return;
        if (view[r][c] != '#') return; 
        
        view[r][c] = board[r][c] + '0';

        if (board[r][c] == 0) {
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i != 0 || j != 0) {
                        floodFill(r + i, c + j);
                    }
                }
            }
        }
    }

    void revealBombs() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == -1) {
                    view[i][j] = '*';
                }
            }
        }
    }

    bool checkWinCondition() {
        int correctFlags = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == -1 && view[i][j] == 'F') correctFlags++;
                if (board[i][j] != -1 && view[i][j] == 'F') return false; 
            }
        }
        return correctFlags == totalBombs;
    }

public:
    Minesweeper() : size(0), totalBombs(0), flagsPlaced(0), gameOver(false) {}

    void setupGame() {
        cout << "\n--- Pengaturan Papan ---\n";
        
        // Handle error tanpa melanggar batasan header
        do {
            cout << "Masukkan ukuran papan N x N (4-10): ";
            if (!(cin >> size)) {
                cout << "Input tidak valid!\n";
                cin.clear();
                while (cin.get() != '\n'); // Membersihkan buffer memori secara manual
                size = 0; 
            } else if (size < 4 || size > 10) {
                cout << "Ukuran harus antara 4 sampai 10!\n";
            }
        } while (size < 4 || size > 10);

        int maxBombs = (size * size) - 1; 
        
        do {
            cout << "Masukkan jumlah bom (1-" << maxBombs << "): ";
            if (!(cin >> totalBombs)) {
                cout << "Input tidak valid!\n";
                cin.clear();
                while (cin.get() != '\n'); 
                totalBombs = 0;
            } else if (totalBombs < 1 || totalBombs > maxBombs) {
                cout << "Jumlah bom di luar batas yang diizinkan!\n";
            }
        } while (totalBombs < 1 || totalBombs > maxBombs);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                board[i][j] = 0;
                view[i][j] = '#';
            }
        }

        int bombsPlanted = 0;
        while (bombsPlanted < totalBombs) {
            int r = rand() % size;
            int c = rand() % size;
            if (board[r][c] != -1) {
                board[r][c] = -1;
                bombsPlanted++;
            }
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] != -1) {
                    board[i][j] = countAdjacentBombs(i, j);
                }
            }
        }

        flagsPlaced = 0;
        gameOver = false;
        startTime = time(0); 
    }

    void printBoard() {
        int elapsedTime = time(0) - startTime;
        cout << "\n================================\n";
        cout << "Waktu berjalan: " << elapsedTime << " detik\n";
        cout << "Bom belum ditandai: " << totalBombs - flagsPlaced << "\n\n";

        cout << "   ";
        for (int c = 0; c < size; c++) cout << c + 1 << " ";
        cout << "\n";

        for (int r = 0; r < size; r++) {
            if (r + 1 < 10) cout << " ";
            cout << r + 1 << " ";
            for (int c = 0; c < size; c++) {
                cout << view[r][c] << " ";
            }
            cout << "\n";
        }
        cout << "================================\n";
    }

    void play() {
        setupGame();
        int r, c;
        int action;

        while (!gameOver) {
            printBoard();
            
            cout << "\nFormat input koordinat: [Baris] [Kolom]\n";
            cout << "Pilih Aksi Pemain:\n1. Buka Kotak\n2. Tandai/Hapus Flag\nPilihan Aksi: ";
            
            if (!(cin >> action)) {
                cout << "Input tidak valid!\n";
                cin.clear();
                while (cin.get() != '\n'); 
                continue;
            }

            if (action != 1 && action != 2) {
                cout << "Aksi tidak valid!\n";
                continue;
            }

            cout << "Masukkan koordinat baris dan kolom: ";
            
            if (!(cin >> r >> c)) {
                cout << "Input tidak valid!.\n";
                cin.clear();
                while (cin.get() != '\n'); 
                continue;
            }

            r--; c--; 

            if (r < 0 || r >= size || c < 0 || c >= size) {
                cout << "Koordinat berada di luar batas papan!\n";
                continue;
            }

            if (action == 1) { 
                if (view[r][c] == 'F') {
                    cout << "Kotak ini sedang ditandai! Hapus Flag terlebih dahulu.\n";
                } else if (board[r][c] == -1) {
                    gameOver = true;
                    revealBombs();
                    printBoard();
                    cout << "\n BOM MELEDAK! GAME OVER.\n";
                    cout << "Total waktu permainan: " << time(0) - startTime << " detik.\n";
                } else {
                    floodFill(r, c); 
                }
            } else if (action == 2) { 
                if (view[r][c] == '#') {
                    view[r][c] = 'F';
                    flagsPlaced++;
                } else if (view[r][c] == 'F') {
                    view[r][c] = '#';
                    flagsPlaced--;
                } else {
                    cout << "Kotak sudah terbuka, tidak bisa ditandai.\n";
                }
            }

            if (!gameOver && checkWinCondition()) {
                gameOver = true;
                printBoard();
                cout << "\n BERHASIL MENANDAI SEMUA BOM!\n";
                cout << "Total waktu permainan: " << time(0) - startTime << " detik.\n";
            }
        }
    }
};

int main() {
    srand(time(0)); 
    int menuChoice;
    Minesweeper game;

    do {
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Mulai Permainan Baru\n";
        cout << "2. Keluar\n";
        cout << "Pilih (1/2): ";
        
        if (!(cin >> menuChoice)) {
            cout << "Input tidak valid!\n";
            cin.clear();
            while (cin.get() != '\n'); 
            continue; 
        }

        if (menuChoice == 1) {
            game.play(); 
        } else if (menuChoice != 2) {
            cout << "Pilihan tidak valid!\n";
        }
    } while (menuChoice != 2);


}