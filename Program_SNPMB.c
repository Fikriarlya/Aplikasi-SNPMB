#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File database
const char* DATABASE = "database_snpmb.txt";
const char* DATABASE_TEMP = "database_snpmb_temp.txt";

// Struct data yang akan diinput
typedef struct {
    int id;
    char nama[100];
    char nisn[11];
    char asal[100];
    char jalur[5];
    char univ[100];
    char jurusan[100];
} Siswa;

// --- FUNGSI UTILITAS (HELPER) ---

// Clear tampilan
void bersihkan_tampilan() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Clear buffer
void bersihkan_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Judul menu
void judul() {
    printf("\n======================================================\n");
    printf("                    APLIKASI SNPMB    \n");
    printf("======================================================\n");
}

// Menu utama aplikasi
int menu() {
    int pilihan;
    printf("\n------- MENU UTAMA -------\n");
    printf("[1] Tambah Data Siswa\n");
    printf("[2] Lihat Data\n");
    printf("[3] Cari Siswa\n");
    printf("[4] Edit Data\n");
    printf("[5] Hapus Data\n");
    printf("[6] Keluar\n");
    printf("Pilih menu: ");
    scanf("%d", &pilihan);
    bersihkan_buffer();
    return pilihan;
}

// Menu untuk sorting data
int sorting() {
    int opsi;
    printf("\n------- PILIH URUTAN -------\n");
    printf("1. ID Terkecil - Terbesar\n");
    printf("2. ID Terbesar - Terkecil\n");
    printf("3. Nama A - Z\n");
    printf("4. Nama Z - A\n");
    printf("Pilih urutan (1-4): ");
    scanf("%d", &opsi);
    bersihkan_buffer();
    return opsi;
}

// Mengecek apakah input angka atau bukan
int is_all_digit(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

// Fungsi untuk mengedit data melalui ID
int minta_id(char* label) {
    int id;
    printf("\nMasukkan ID Siswa yang ingin %s: ", label);
    scanf("%d", &id);
    bersihkan_buffer();
    return id;
}

// Fungsi untuk mencari nama siswa di data
void minta_kata_kunci(char* buffer) {
    printf("\nCari Nama Siswa: ");
    scanf("%[^\n]", buffer);
    bersihkan_buffer();
}

// Form input untuk tambah data siswa
void form_input(char* nama, char* nisn, char* asal, char* jalur, char* univ, char* jurusan) {
    printf("\n--- INPUT DATA SNPMB ---\n");
    printf("1. Nama Lengkap   : "); 
    scanf("%[^\n]", nama); 
    bersihkan_buffer();
    
    // Validasi NISN (10 digit)
    char temp_NISN[50];
    int valid = 0;
    do {
        printf("2. NISN (10 Digit): "); 
        scanf("%s", temp_NISN); 
        bersihkan_buffer();
        if (strlen(temp_NISN) != 10) {
            printf("   [ERROR] Harus 10 digit!\n");
        } else if (!is_all_digit(temp_NISN)) {
            printf("   [ERROR] Harus angka!\n");
        } else {
            strcpy(nisn, temp_NISN);
            valid = 1;
        }
    } while (!valid);

    printf("3. Asal Sekolah   : "); 
    scanf("%[^\n]", asal); 
    bersihkan_buffer();
    
    // Validasi Jalur (SNBP/SNBT)
    char temp_Jalur[50];
    int valid_Jalur = 0;
    do {
        printf("4. Jalur (SNBP/SNBT): "); 
        scanf("%s", temp_Jalur); 
        bersihkan_buffer();
        if (strcmp(temp_Jalur, "SNBP") == 0 || strcmp(temp_Jalur, "SNBT") == 0) {
            strcpy(jalur, temp_Jalur);
            valid_Jalur = 1;
        } else {
            printf("   [ERROR] Isi SNBP atau SNBT (Kapital)!\n");
        }
    } while (!valid_Jalur);

    printf("5. Univ. Tujuan   : "); 
    scanf("%[^\n]", univ); 
    bersihkan_buffer();
    
    printf("6. Jurusan        : "); 
    scanf("%[^\n]", jurusan); 
    bersihkan_buffer();
}

// Header tabel
void header_tabel() {
    printf("\n%-3s| %-15s| %-12s| %-15s| %-6s| %-12s| %-15s\n", 
           "ID", "NAMA", "NISN", "SEKOLAH", "JALUR", "UNIV", "JURUSAN");
    printf("----------------------------------------------------------------------------------------\n");
}

// Tabel data
void baris_data(int id, char* nama, char* nisn, char* asal, char* jalur, char* univ, char* jurusan) {
    printf("%-3d| %-15s| %-12s| %-15s| %-6s| %-12s| %-15s\n", 
           id, nama, nisn, asal, jalur, univ, jurusan);
}

// Auto id untuk siswa selanjutnya
/* Memakai pointer, buka file database (database_snpmb.txt) dengan mode read, jika pointer tidak = null maka kembali, membaca database,
ketika id yang ada di database lebih besar daripada max_id, maka max_id + 1 sehingga siswa selanjutnya akan mendapat id +1 dengan otomatis*/ 
int auto_id() {
    FILE *fp = fopen(DATABASE, "r");
    if (fp == NULL) return 1;

    Siswa temp;
    int max_id = 0;
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", 
                  &temp.id, temp.nama, temp.nisn, temp.asal, 
                  temp.jalur, temp.univ, temp.jurusan) != EOF) {
        if (temp.id > max_id) {
            max_id = temp.id;
        }
    }
    fclose(fp);
    return max_id + 1;
}

// --- FITUR UTAMA ---

// CASE 1 - Tambah data siswa
void tambah_data() {
    Siswa input;
    FILE *fp;

    form_input(input.nama, input.nisn, input.asal, input.jalur, input.univ, input.jurusan);
    
    int id_baru = auto_id();
    
    fp = fopen(DATABASE, "a");
    if (fp == NULL) {
        printf("Error: Tidak dapat membuka file!\n");
        return;
    }
    
    fprintf(fp, "%d,%s,%s,%s,%s,%s,%s\n", 
            id_baru, input.nama, input.nisn, input.asal, 
            input.jalur, input.univ, input.jurusan);
    fclose(fp);
    
    printf("\n>> Data tersimpan dengan ID: %d\n", id_baru);
    bersihkan_tampilan();
}

// CASE 2 - Lihat data siswa + sorting
void lihat_data() {
    Siswa list_siswa[100];
    FILE *fp;
    int jumlah_data = 0;
    
    // Baca semua data
    fp = fopen(DATABASE, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", 
                      &list_siswa[jumlah_data].id, 
                      list_siswa[jumlah_data].nama, 
                      list_siswa[jumlah_data].nisn, 
                      list_siswa[jumlah_data].asal, 
                      list_siswa[jumlah_data].jalur, 
                      list_siswa[jumlah_data].univ, 
                      list_siswa[jumlah_data].jurusan) != EOF) {
            jumlah_data++;
        }
        fclose(fp);
    }
    
    if (jumlah_data == 0) {
        printf("\n  (Belum ada data)\n");
        printf("\nTekan Enter untuk kembali...");
        getchar();
        return;
    }
    
    // Sorting data menggunakan bubble sort
    int mode = sorting();
    Siswa temp;
    
    for (int i = 0; i < jumlah_data - 1; i++) {
        for (int j = 0; j < jumlah_data - i - 1; j++) {
            int harus_tukar = 0;
            
            if (mode == 1) {
                // ID Ascending
                if (list_siswa[j].id > list_siswa[j+1].id) harus_tukar = 1;
            } else if (mode == 2) {
                // ID Descending
                if (list_siswa[j].id < list_siswa[j+1].id) harus_tukar = 1;
            } else if (mode == 3) {
                // Nama A-Z
                if (strcmp(list_siswa[j].nama, list_siswa[j+1].nama) > 0) harus_tukar = 1;
            } else if (mode == 4) {
                // Nama Z-A
                if (strcmp(list_siswa[j].nama, list_siswa[j+1].nama) < 0) harus_tukar = 1;
            }
            
            // Eksekusi tukar
            if (harus_tukar) {
                temp = list_siswa[j];
                list_siswa[j] = list_siswa[j+1];
                list_siswa[j+1] = temp;
            }
        }
    }
    
    // Tampilkan data
    header_tabel();
    for (int i = 0; i < jumlah_data; i++) {
        baris_data(list_siswa[i].id, list_siswa[i].nama, list_siswa[i].nisn, 
                   list_siswa[i].asal, list_siswa[i].jalur, 
                   list_siswa[i].univ, list_siswa[i].jurusan);
    }
    
    printf("\nTotal: %d siswa\n", jumlah_data);
    printf("\nTekan Enter untuk keluar...");
    getchar();
    bersihkan_tampilan();
}

// CASE 3 - Cari data berdasarkan nama siswa
void cari_data() {
    Siswa baca;
    FILE *fp;
    char keyword[100];
    int ketemu = 0;

    minta_kata_kunci(keyword);
    
    fp = fopen(DATABASE, "r");
    if (fp == NULL) {
        printf("Error: Tidak dapat membuka file!\n");
        return;
    }
    
    header_tabel();
    
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", 
                  &baca.id, baca.nama, baca.nisn, baca.asal, 
                  baca.jalur, baca.univ, baca.jurusan) != EOF) {
        if (strstr(baca.nama, keyword) != NULL) {
            baris_data(baca.id, baca.nama, baca.nisn, baca.asal, 
                      baca.jalur, baca.univ, baca.jurusan);
            ketemu = 1;
        }
    }
    fclose(fp);
    
    if (!ketemu) {
        printf("  Data tidak ditemukan.\n");
    }
    
    printf("\nTekan Enter untuk keluar...");
    getchar();
    bersihkan_tampilan();
}

// CASE 4 - Edit data siswa
void edit_data() {
    Siswa input, baca;
    FILE *fp, *fp_temp;
    int target_id;
    int found = 0;

    target_id = minta_id("di-edit");
    
    fp = fopen(DATABASE, "r");
    fp_temp = fopen(DATABASE_TEMP, "w");
    
    if (fp == NULL || fp_temp == NULL) {
        printf("Error: Tidak dapat membuka file!\n");
        if (fp) fclose(fp);
        if (fp_temp) fclose(fp_temp);
        return;
    }
    
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", 
                  &baca.id, baca.nama, baca.nisn, baca.asal, 
                  baca.jalur, baca.univ, baca.jurusan) != EOF) {
        if (baca.id == target_id) {
            printf("\n>> Mengedit data: %s\n", baca.nama);
            form_input(input.nama, input.nisn, input.asal, 
                      input.jalur, input.univ, input.jurusan);
            fprintf(fp_temp, "%d,%s,%s,%s,%s,%s,%s\n", 
                    target_id, input.nama, input.nisn, input.asal, 
                    input.jalur, input.univ, input.jurusan);
            found = 1;
        } else {
            fprintf(fp_temp, "%d,%s,%s,%s,%s,%s,%s\n", 
                    baca.id, baca.nama, baca.nisn, baca.asal, 
                    baca.jalur, baca.univ, baca.jurusan);
        }
    }
    
    fclose(fp);
    fclose(fp_temp);
    
    if (found) {
        remove(DATABASE);
        rename(DATABASE_TEMP, DATABASE);
        printf("\n>> Data berhasil diupdate!\n");
    } else {
        remove(DATABASE_TEMP);
        printf("\n>> ID tidak ditemukan.\n");
    }
    bersihkan_tampilan();
}

// CASE 5 - Hapus data siswa
void hapus_data() {
    Siswa baca;
    FILE *fp, *fp_temp;
    int target_id;
    int found = 0;

    target_id = minta_id("dihapus");
    
    fp = fopen(DATABASE, "r");
    fp_temp = fopen(DATABASE_TEMP, "w");
    
    if (fp == NULL || fp_temp == NULL) {
        printf("Error: Tidak dapat membuka file!\n");
        if (fp) fclose(fp);
        if (fp_temp) fclose(fp_temp);
        return;
    }
    
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", 
                  &baca.id, baca.nama, baca.nisn, baca.asal, 
                  baca.jalur, baca.univ, baca.jurusan) != EOF) {
        if (baca.id == target_id) {
            found = 1;
            printf("\n>> Menghapus data siswa: %s\n", baca.nama);
        } else {
            fprintf(fp_temp, "%d,%s,%s,%s,%s,%s,%s\n", 
                    baca.id, baca.nama, baca.nisn, baca.asal, 
                    baca.jalur, baca.univ, baca.jurusan);
        }
    }
    
    fclose(fp);
    fclose(fp_temp);
    
    if (found) {
        remove(DATABASE);
        rename(DATABASE_TEMP, DATABASE);
        printf(">> Data berhasil dihapus!\n");
    } else {
        remove(DATABASE_TEMP);
        printf(">> ID tidak ditemukan.\n");
    }
    bersihkan_tampilan();
}

int main() {
    int pilihan;

    while (1) {
        judul();
        pilihan = menu();

        switch (pilihan) {
            case 1:
                tambah_data();
                break;
            case 2:
                lihat_data();
                break;
            case 3:
                cari_data();
                break;
            case 4:
                edit_data();
                break;
            case 5:
                hapus_data();
                break;
            case 6:
                printf("\nTerima kasih telah menggunakan aplikasi.\n");
                return 0;
            default:
                printf("\nPilihan tidak valid!\n");
                printf("\nTekan Enter untuk melanjutkan...");
                getchar();
        }
    }
    return 0;
}