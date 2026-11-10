/*
 * ====================================================================
 *  QUAN LY PHONG TRA TU SA - Lap trinh C/C++ co ban (Sinh vien nam nhat)
 * --------------------------------------------------------------------
 *  Kien thuc su dung:
 *   - Cau truc struct TeaPot + mang tinh 1 chieu arr[100] + bien dem n.
 *   - Chuoi ky tu C char[] voi <string.h>: strcpy, strcmp, strstr, strlen.
 *   - Xuat bang can cot bang printf("%-8d ...") cua <stdio.h>.
 *   - Nhap du lieu an toan bang cin / cin.getline + cin.ignore.
 *   - Truyen tham chieu (int &n, double &doanhThu) cua C++ co ban.
 *   - Doc / Ghi file nhi phan bang FILE*, fopen, fread, fwrite.
 * ====================================================================
 */

#include <stdio.h>    // printf, fflush, FILE, fopen, fread, fwrite, fclose
#include <string.h>   // strcpy, strcmp, strstr, strlen
#include <stdlib.h>   // exit
#include <iostream>   // cin, cin.getline, cin.ignore
using namespace std;

#define MAX 100              // So am toi da trong phong tra
#define DO_RONG 94           // Do rong duong ke bang
#define MUC_BAO_TRI 10       // Cu 10 lan dung thi khi tra am se chuyen sang Bao tri
#define TEN_FILE "phongtra.dat"

// Ba trang thai hop le cua mot am tra
#define TT_SAN_SANG  "San sang"
#define TT_DANG_DUNG "Dang dung"
#define TT_BAO_TRI   "Bao tri"

struct TeaPot {
    int id;                 // Ma am (1001, 1002...)
    char name[30];          // Ten dang am (Tay Thi, Thach Bieu...)
    char material[25];      // Chat lieu dat (Tu Ne, Chu Ne, Doan Ne...)
    int capacity;           // Dung tich (ml)
    double price;           // Gia dich vu (VND)
    char status[20];        // Trang thai ("San sang", "Dang dung", "Bao tri")
    int usageCount;         // So lan da su dung
};

/* ================= PHAN 1: HAM TIEN ICH NHAP / XUAT ================= */

// Het du lieu nhap (Ctrl+Z / Ctrl+D) thi thoat, tranh lap vo han
void kiemTraHetDuLieu() {
    if (cin.eof()) {
        printf("\nKhong con du lieu nhap. Thoat chuong trinh.\n");
        exit(0);
    }
}

// Nhap 1 so nguyen trong khoang [nho, lon]. Nhap sai thi bat nhap lai.
int nhapSoNguyen(const char thongBao[], int nho, int lon) {
    int x;
    while (1) {
        printf("%s", thongBao);
        fflush(stdout);
        cin >> x;
        kiemTraHetDuLieu();
        if (cin.fail()) {                // Nguoi dung go chu thay vi so
            cin.clear();                 // Xoa co loi cua cin
            cin.ignore(10000, '\n');     // Bo phan con thua tren dong
            printf("  >> Loi: ban phai nhap mot SO nguyen!\n");
            continue;
        }
        // Xoa ky tu Enter ('\n') con trong bo nho dem de khong troi cin.getline()
        cin.ignore(10000, '\n');
        if (x < nho || x > lon) {
            printf("  >> Loi: gia tri phai tu %d den %d!\n", nho, lon);
            continue;
        }
        return x;
    }
}

// Nhap 1 so thuc (double) trong khoang [nho, lon]
double nhapSoThuc(const char thongBao[], double nho, double lon) {
    double x;
    while (1) {
        printf("%s", thongBao);
        fflush(stdout);
        cin >> x;
        kiemTraHetDuLieu();
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            printf("  >> Loi: ban phai nhap mot SO!\n");
            continue;
        }
        cin.ignore(10000, '\n');
        if (x < nho || x > lon) {
            printf("  >> Loi: gia tri phai tu %.0f den %.0f!\n", nho, lon);
            continue;
        }
        return x;
    }
}

// Nhap 1 chuoi co khoang trang (khong de trong, toi da doDai-1 ky tu)
void nhapChuoi(const char thongBao[], char s[], int doDai) {
    while (1) {
        printf("%s", thongBao);
        fflush(stdout);
        cin.getline(s, doDai);           // Doc ca dong, ke ca dau cach
        kiemTraHetDuLieu();
        if (cin.fail()) {                // Go qua dai so voi mang -> cin bi loi
            cin.clear();
            cin.ignore(10000, '\n');
            printf("  >> Loi: chuoi qua dai (toi da %d ky tu)!\n", doDai - 1);
            continue;
        }
        if (strlen(s) == 0) {            // strlen(s): do dai chuoi s (khong tinh '\0')
            printf("  >> Loi: khong duoc de trong!\n");
            continue;
        }
        return;
    }
}

// Chuyen chuoi 'nguon' thanh chu thuong, luu vao 'dich'
// (dung de tim kiem va sap xep khong phan biet chu hoa / chu thuong)
void chuyenThuong(const char nguon[], char dich[]) {
    int i = 0;
    while (nguon[i] != '\0') {
        if (nguon[i] >= 'A' && nguon[i] <= 'Z')
            dich[i] = nguon[i] + 32;     // Trong bang ASCII: 'A'(65) + 32 = 'a'(97)
        else
            dich[i] = nguon[i];
        i++;
    }
    dich[i] = '\0';                      // Ket thuc chuoi bang ky tu null '\0'
}

// Cho chon 1 trong 3 trang thai, ghi ket qua vao chuoi 'dich'
void chonTrangThai(char dich[]) {
    printf("   1. %s\n   2. %s\n   3. %s\n", TT_SAN_SANG, TT_DANG_DUNG, TT_BAO_TRI);
    int c = nhapSoNguyen("   Chon trang thai (1-3): ", 1, 3);
    if (c == 1)
        strcpy(dich, TT_SAN_SANG);       // strcpy(dich, nguon): chep chuoi nguon vao dich
    else if (c == 2)
        strcpy(dich, TT_DANG_DUNG);
    else
        strcpy(dich, TT_BAO_TRI);
}

// In 1 duong ke ngang gom 'doDai' dau gach
void inDuongKe(int doDai) {
    for (int i = 0; i < doDai; i++)
        printf("-");
    printf("\n");
}

// In dong tieu de cua bang (%-Ns: can le trai, N la do rong cot)
void inTieuDe() {
    inDuongKe(DO_RONG);
    printf("%-6s %-24s %-12s %-13s %-14s %-11s %-8s\n",
           "ID", "TEN DANG AM", "CHAT LIEU", "DUNG TICH(ml)", "GIA (VND)", "TRANG THAI", "SO LAN");
    inDuongKe(DO_RONG);
}

// In 1 dong du lieu cua 1 chiec am
void inMotAm(TeaPot a) {
    printf("%-6d %-24s %-12s %-13d %-14.0f %-11s %-8d\n",
           a.id, a.name, a.material, a.capacity, a.price, a.status, a.usageCount);
}

// In toan bo danh sach am tra
void inDanhSach(TeaPot arr[], int n) {
    if (n == 0) {
        printf("\n>> Phong tra chua co chiec am nao!\n");
        return;
    }
    printf("\n===== DANH SACH AM TRA TU SA =====\n");
    inTieuDe();
    for (int i = 0; i < n; i++)
        inMotAm(arr[i]);
    inDuongKe(DO_RONG);
    printf("Tong cong: %d am\n", n);
}

// In cac am co trang thai 'trangThai'. Tra ve so am da in.
int inAmTheoTrangThai(TeaPot arr[], int n, const char trangThai[]) {
    int dem = 0;
    for (int i = 0; i < n; i++) {
        // strcmp(a, b) tra ve 0 neu hai chuoi GIONG HET nhau
        if (strcmp(arr[i].status, trangThai) == 0) {
            if (dem == 0)
                inTieuDe();
            inMotAm(arr[i]);
            dem++;
        }
    }
    if (dem > 0)
        inDuongKe(DO_RONG);
    return dem;
}

/* ============ PHAN 2: DOC/GHI FILE, KHOI TAO & QUAN LY (THEM / SUA / XOA) ============ */

// Ghi toan bo danh sach am va doanh thu ra file nhi phan "phongtra.dat"
void luuFile(TeaPot arr[], int n, double doanhThu, int soLuotThue) {
    FILE *f = fopen(TEN_FILE, "wb");     // "wb": write binary (ghi nhi phan)
    if (f == NULL) {
        printf(">> Loi: Khong the mo file %s de ghi!\n", TEN_FILE);
        return;
    }
    fwrite(&n, sizeof(int), 1, f);               // Ghi so luong am n
    fwrite(&doanhThu, sizeof(double), 1, f);     // Ghi tong doanh thu
    fwrite(&soLuotThue, sizeof(int), 1, f);      // Ghi tong so luot thue
    if (n > 0) {
        fwrite(arr, sizeof(TeaPot), n, f);       // Ghi nguyen mang n chiec am
    }
    fclose(f);
}

// Doc du lieu tu file nhi phan len mang. Tra ve 1 neu doc thanh cong, 0 neu chua co file
int docFile(TeaPot arr[], int &n, double &doanhThu, int &soLuotThue) {
    FILE *f = fopen(TEN_FILE, "rb");     // "rb": read binary (doc nhi phan)
    if (f == NULL) {
        return 0;                        // Chua co file (lan chay dau tien)
    }
    if (fread(&n, sizeof(int), 1, f) != 1 || n < 0 || n > MAX) {
        fclose(f);
        n = 0;
        return 0;
    }
    fread(&doanhThu, sizeof(double), 1, f);
    fread(&soLuotThue, sizeof(int), 1, f);
    if (n > 0) {
        fread(arr, sizeof(TeaPot), n, f);
    }
    fclose(f);
    return 1;
}

// Them 1 am co san thong tin vao mang (dung tao du lieu mau)
void themAmMau(TeaPot arr[], int &n, int id, const char ten[], const char chatLieu[],
               int dungTich, double gia, const char trangThai[], int soLan) {
    if (n >= MAX)
        return;
    arr[n].id = id;
    strcpy(arr[n].name, ten);
    strcpy(arr[n].material, chatLieu);
    arr[n].capacity = dungTich;
    arr[n].price = gia;
    strcpy(arr[n].status, trangThai);
    arr[n].usageCount = soLan;
    n++;
}

// Nap san 8 chiec am mau de chay la co du lieu thu
void khoiTaoDuLieu(TeaPot arr[], int &n) {
    themAmMau(arr, n, 1001, "Tay Thi",     "Tu Ne",   150, 350000, TT_SAN_SANG,  4);
    themAmMau(arr, n, 1002, "Thach Bieu",  "Chu Ne",  220, 450000, TT_DANG_DUNG, 7);
    themAmMau(arr, n, 1003, "Phuc Dang",   "Doan Ne", 180, 520000, TT_SAN_SANG,  3);
    themAmMau(arr, n, 1004, "Ban Nguyet",  "Tu Ne",   120, 300000, TT_BAO_TRI,  10);
    themAmMau(arr, n, 1005, "Duong Phien", "Chu Ne",  250, 480000, TT_SAN_SANG,  8);
    themAmMau(arr, n, 1006, "Long Dan",    "Doan Ne", 300, 650000, TT_SAN_SANG,  2);
    themAmMau(arr, n, 1007, "Cung Xuan",   "Tu Ne",   160, 380000, TT_SAN_SANG,  5);
    themAmMau(arr, n, 1008, "Bao Chau",    "Chu Ne",  360, 550000, TT_SAN_SANG,  6);
}

// Tim am theo ma. Tra ve vi tri trong mang, khong thay thi tra ve -1
int timTheoId(TeaPot arr[], int n, int id) {
    for (int i = 0; i < n; i++) {
        if (arr[i].id == id)
            return i;
    }
    return -1;
}

// Hoi nguoi dung ma am, tra ve vi tri trong mang (-1 neu khong co)
int nhapVaTimId(TeaPot arr[], int n) {
    int id = nhapSoNguyen("Nhap ma am: ", 1, 999999);
    int vt = timTheoId(arr, n, id);
    if (vt == -1)
        printf(">> Khong tim thay am co ma %d!\n", id);
    return vt;
}

// Ma moi = ma lon nhat hien co + 1 (danh sach rong thi bat dau tu 1001)
int taoIdMoi(TeaPot arr[], int n) {
    if (n == 0)
        return 1001;
    int maxId = arr[0].id;
    for (int i = 1; i < n; i++) {
        if (arr[i].id > maxId)
            maxId = arr[i].id;
    }
    return maxId + 1;
}

// Chuc nang 2: Them am moi
void themAm(TeaPot arr[], int &n) {
    printf("\n===== THEM AM MOI =====\n");
    if (n >= MAX) {
        printf(">> Phong tra da day (%d am), khong the them nua!\n", MAX);
        return;
    }
    TeaPot a;
    a.id = taoIdMoi(arr, n);
    printf("Ma am moi (tu dong tao): %d\n", a.id);
    nhapChuoi("Nhap ten dang am: ", a.name, 30);
    nhapChuoi("Nhap chat lieu dat (Tu Ne / Chu Ne / Doan Ne...): ", a.material, 25);
    a.capacity = nhapSoNguyen("Nhap dung tich (ml, tu 50 den 1000): ", 50, 1000);
    a.price = nhapSoThuc("Nhap gia dich vu (VND): ", 1000, 100000000);
    strcpy(a.status, TT_SAN_SANG);       // Am moi luon o trang thai san sang
    a.usageCount = 0;
    arr[n] = a;                          // Gan ca struct vao o cuoi mang
    n++;
    printf(">> Da them thanh cong am \"%s\" (ma %d)!\n", a.name, a.id);
}

// Chuc nang 4: Sua thong tin 1 chiec am
void suaAm(TeaPot arr[], int n) {
    printf("\n===== SUA THONG TIN AM =====\n");
    if (n == 0) {
        printf(">> Phong tra chua co am nao!\n");
        return;
    }
    int vt = nhapVaTimId(arr, n);
    if (vt == -1)
        return;

    int chon;
    do {
        printf("\nThong tin hien tai:\n");
        inTieuDe();
        inMotAm(arr[vt]);
        inDuongKe(DO_RONG);
        printf("  1. Ten dang am     2. Chat lieu    3. Dung tich\n");
        printf("  4. Gia dich vu     5. Trang thai   0. Quay lai\n");
        chon = nhapSoNguyen("Chon muc can sua: ", 0, 5);
        switch (chon) {
            case 1: nhapChuoi("Ten moi: ", arr[vt].name, 30); break;
            case 2: nhapChuoi("Chat lieu moi: ", arr[vt].material, 25); break;
            case 3: arr[vt].capacity = nhapSoNguyen("Dung tich moi (ml): ", 50, 1000); break;
            case 4: arr[vt].price = nhapSoThuc("Gia moi (VND): ", 1000, 100000000); break;
            case 5: chonTrangThai(arr[vt].status); break;
        }
        if (chon != 0)
            printf(">> Da cap nhat!\n");
    } while (chon != 0);
}

// Chuc nang 5: Xoa am (don cac phan tu phia sau len 1 o)
void xoaAm(TeaPot arr[], int &n) {
    printf("\n===== XOA AM =====\n");
    if (n == 0) {
        printf(">> Phong tra chua co am nao!\n");
        return;
    }
    int vt = nhapVaTimId(arr, n);
    if (vt == -1)
        return;
    if (strcmp(arr[vt].status, TT_DANG_DUNG) == 0) {
        printf(">> Am dang co khach su dung, khong the xoa!\n");
        return;
    }
    inTieuDe();
    inMotAm(arr[vt]);
    inDuongKe(DO_RONG);
    if (nhapSoNguyen("Ban chac chan xoa? (1: Co, 0: Khong): ", 0, 1) == 0) {
        printf(">> Da huy thao tac xoa.\n");
        return;
    }
    for (int i = vt; i < n - 1; i++)     // Phan tu sau de len phan tu truoc
        arr[i] = arr[i + 1];
    n--;                                 // Giam so luong thuc te
    printf(">> Da xoa am thanh cong!\n");
}

/* ============================ PHAN 3: TIM KIEM ============================ */

// Tim theo ten (theoTen = 1: chi can chua tu khoa, dung strstr)
// hoac theo chat lieu (theoTen = 0: phai giong het, dung strcmp).
void timTheoChuoi(TeaPot arr[], int n, int theoTen) {
    char tuKhoa[30], khoaThuong[30], truongThuong[30];
    if (theoTen == 1)
        nhapChuoi("Nhap tu khoa trong ten am: ", tuKhoa, 30);
    else
        nhapChuoi("Nhap chat lieu can tim: ", tuKhoa, 30);
    chuyenThuong(tuKhoa, khoaThuong);

    int dem = 0;
    for (int i = 0; i < n; i++) {
        int khop;
        if (theoTen == 1) {
            chuyenThuong(arr[i].name, truongThuong);
            // strstr(a, b): tim chuoi b nam ben trong chuoi a
            khop = (strstr(truongThuong, khoaThuong) != NULL);
        } else {
            chuyenThuong(arr[i].material, truongThuong);
            khop = (strcmp(truongThuong, khoaThuong) == 0);
        }
        if (khop == 1) {
            if (dem == 0)
                inTieuDe();
            inMotAm(arr[i]);
            dem++;
        }
    }
    if (dem == 0)
        printf(">> Khong tim thay am nao khop voi \"%s\"!\n", tuKhoa);
    else {
        inDuongKe(DO_RONG);
        printf("Tim thay %d am.\n", dem);
    }
}

// Tim va loc am tra theo so nguoi uong (Cong thuc: searchVol = numPeople * 60 + 30)
void timTheoSoNguoiUong(TeaPot arr[], int n) {
    int soNguoi = nhapSoNguyen("Nhap so nguoi uong tra (1-20): ", 1, 20);
    int searchVol = soNguoi * 60 + 30;

    printf("\n---------------- THONG TIN TINH TOAN DUNG TICH ----------------\n");
    printf(" - So nguoi uong            : %d nguoi\n", soNguoi);
    printf(" - Dung tich chen tra       : %d x 60 ml = %d ml\n", soNguoi, soNguoi * 60);
    printf(" - Dung tich tra no trong am: 30 ml\n");
    printf(" => Dung tich am toi thieu can tim (capacity >= %d ml)\n", searchVol);

    int dem = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].capacity >= searchVol) {
            if (dem == 0)
                inTieuDe();
            inMotAm(arr[i]);
            dem++;
        }
    }
    if (dem == 0)
        printf(">> Khong co am nao co dung tich >= %d ml!\n", searchVol);
    else {
        inDuongKe(DO_RONG);
        printf("=> Tim thay tong cong %d am tra phu hop.\n", dem);
    }
}

// Chuc nang 3: Menu con tim kiem
void timKiem(TeaPot arr[], int n) {
    printf("\n===== TIM KIEM AM =====\n");
    if (n == 0) {
        printf(">> Phong tra chua co am nao!\n");
        return;
    }
    printf("  1. Theo ma am\n");
    printf("  2. Theo ten (go mot phan ten cung duoc)\n");
    printf("  3. Theo chat lieu\n");
    printf("  4. Theo trang thai\n");
    printf("  5. Theo so nguoi uong (tu dong tinh dung tich: so nguoi * 60 + 30)\n");
    int chon = nhapSoNguyen("Chon cach tim (1-5): ", 1, 5);
    if (chon == 1) {
        int vt = nhapVaTimId(arr, n);
        if (vt != -1) {
            inTieuDe();
            inMotAm(arr[vt]);
            inDuongKe(DO_RONG);
        }
    } else if (chon == 2) {
        timTheoChuoi(arr, n, 1);
    } else if (chon == 3) {
        timTheoChuoi(arr, n, 0);
    } else if (chon == 4) {
        char tt[20];
        chonTrangThai(tt);
        if (inAmTheoTrangThai(arr, n, tt) == 0)
            printf(">> Khong co am nao o trang thai \"%s\"!\n", tt);
    } else {
        timTheoSoNguoiUong(arr, n);
    }
}

/* ======================= PHAN 4: SAP XEP (DOI CHO TRUC TIEP) ======================= */

// Chuc nang 9: Sap xep danh sach theo nhieu tieu chi (Interchange Sort)
void sapXep(TeaPot arr[], int n) {
    printf("\n===== SAP XEP DANH SACH =====\n");
    if (n < 2) {
        printf(">> Can it nhat 2 am de sap xep!\n");
        return;
    }
    printf("  1. Gia tang dan           2. Gia giam dan\n");
    printf("  3. So lan dung giam dan   4. Ten A -> Z\n");
    printf("  5. Dung tich tang dan\n");
    int kieu = nhapSoNguyen("Chon cach sap xep (1-5): ", 1, 5);

    char tenI[30], tenJ[30];
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int doiCho = 0;              // 1 nghia la arr[i], arr[j] dang sai thu tu
            if (kieu == 1 && arr[j].price < arr[i].price)
                doiCho = 1;
            else if (kieu == 2 && arr[j].price > arr[i].price)
                doiCho = 1;
            else if (kieu == 3 && arr[j].usageCount > arr[i].usageCount)
                doiCho = 1;
            else if (kieu == 4) {
                // Chuyen ve chu thuong truoc khi so sanh de "an lac" dung truoc "Tay Thi"
                chuyenThuong(arr[i].name, tenI);
                chuyenThuong(arr[j].name, tenJ);
                if (strcmp(tenJ, tenI) < 0)
                    doiCho = 1;
            }
            else if (kieu == 5 && arr[j].capacity < arr[i].capacity)
                doiCho = 1;

            if (doiCho == 1) {           // Hoan doi 2 struct qua bien trung gian
                TeaPot tam = arr[i];
                arr[i] = arr[j];
                arr[j] = tam;
            }
        }
    }
    printf(">> Da sap xep xong!\n");
    inDanhSach(arr, n);
}

/* ============ PHAN 5: NGHIEP VU PHONG TRA (THUE / TRA / BAO TRI) ============ */

// Hien cac am dang o 'trangThai', cho nhap ma va kiem tra ma do co dung trang thai khong.
int chonAmTheoTrangThai(TeaPot arr[], int n, const char trangThai[]) {
    printf("Cac am dang \"%s\":\n", trangThai);
    if (inAmTheoTrangThai(arr, n, trangThai) == 0) {
        printf(">> Hien khong co am nao o trang thai nay!\n");
        return -1;
    }
    int vt = nhapVaTimId(arr, n);
    if (vt == -1)
        return -1;
    if (strcmp(arr[vt].status, trangThai) != 0) {
        printf(">> Am nay dang \"%s\", khong phu hop!\n", arr[vt].status);
        return -1;
    }
    return vt;
}

// Chuc nang 6: Cho thue am (Tich hop tu van dung tich theo so nguoi uong)
void choThueAm(TeaPot arr[], int n, double &doanhThu, int &soLuotThue) {
    printf("\n===== CHO THUE AM & TU VAN BAN TRA =====\n");
    int soNguoi = nhapSoNguyen("Nhap so nguoi uong tra (nhap 0 neu xem tat ca am San sang): ", 0, 20);
    int dungTichCan = 0;

    if (soNguoi > 0) {
        dungTichCan = soNguoi * 60 + 30;
        printf("=> Goi y: %d nguoi x 60ml + 30ml tra no = Can am toi thieu %d ml.\n\n",
               soNguoi, dungTichCan);
    }

    // Loc va in cac am dang SAN SANG va du dung tich
    int dem = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].status, TT_SAN_SANG) == 0 && arr[i].capacity >= dungTichCan) {
            if (dem == 0)
                inTieuDe();
            inMotAm(arr[i]);
            dem++;
        }
    }

    if (dem == 0) {
        printf(">> Khong co am nao SAN SANG dap ung dung tich >= %d ml!\n", dungTichCan);
        return;
    }
    inDuongKe(DO_RONG);

    int vt = nhapVaTimId(arr, n);
    if (vt == -1)
        return;
    if (strcmp(arr[vt].status, TT_SAN_SANG) != 0) {
        printf(">> Am nay dang \"%s\", khong the cho thue!\n", arr[vt].status);
        return;
    }
    if (arr[vt].capacity < dungTichCan) {
        printf(">> Canh bao: Am nay (%d ml) nho hon dung tich goi y (%d ml)!\n",
               arr[vt].capacity, dungTichCan);
        if (nhapSoNguyen("Khach van muon chon am nay? (1: Co, 0: Huy): ", 0, 1) == 0)
            return;
    }

    strcpy(arr[vt].status, TT_DANG_DUNG);    // Doi trang thai
    arr[vt].usageCount++;                    // Tang so lan su dung
    doanhThu += arr[vt].price;               // Cong tien vao doanh thu
    soLuotThue++;

    printf("\n--------- HOA DON ---------\n");
    printf("Am        : %s (ma %d)\n", arr[vt].name, arr[vt].id);
    printf("Chat lieu : %s - %d ml\n", arr[vt].material, arr[vt].capacity);
    printf("Thanh tien: %.0f VND\n", arr[vt].price);
    printf("---------------------------\n");
    printf(">> Cho thue thanh cong! (Lan dung thu %d)\n", arr[vt].usageCount);
}

// Chuc nang 7: Khach tra am
void nhanTraAm(TeaPot arr[], int n) {
    printf("\n===== NHAN TRA AM =====\n");
    int vt = chonAmTheoTrangThai(arr, n, TT_DANG_DUNG);
    if (vt == -1)
        return;
    // Dung du MUC_BAO_TRI lan (10, 20, 30...) thi phai bao tri
    if (arr[vt].usageCount % MUC_BAO_TRI == 0) {
        strcpy(arr[vt].status, TT_BAO_TRI);
        printf(">> Da nhan lai am. Am da dung %d lan nen chuyen sang BAO TRI.\n",
               arr[vt].usageCount);
    } else {
        strcpy(arr[vt].status, TT_SAN_SANG);
        printf(">> Da nhan lai am. Am tro ve trang thai SAN SANG.\n");
    }
}

// Chuc nang 8: Bao tri xong, dua am ve trang thai san sang
void hoanTatBaoTri(TeaPot arr[], int n) {
    printf("\n===== HOAN TAT BAO TRI =====\n");
    int vt = chonAmTheoTrangThai(arr, n, TT_BAO_TRI);
    if (vt == -1)
        return;
    strcpy(arr[vt].status, TT_SAN_SANG);
    printf(">> Am \"%s\" da bao tri xong va san sang phuc vu khach!\n", arr[vt].name);
}

/* ============================ PHAN 6: THONG KE ============================ */

// Chuc nang 10: Thong ke tong hop
void thongKe(TeaPot arr[], int n, double doanhThu, int soLuotThue) {
    printf("\n===== THONG KE PHONG TRA TU SA =====\n");
    if (n == 0) {
        printf(">> Phong tra chua co am nao de thong ke!\n");
        return;
    }
    int demSanSang = 0, demDangDung = 0, demBaoTri = 0;
    int tongLuotDung = 0;
    double tongGia = 0;
    int iDat = 0, iRe = 0, iNhieu = 0;   // Vi tri am dat nhat, re nhat, dung nhieu nhat

    // 3 chat lieu pho bien (viet thuong de so sanh) va bo dem tuong ung
    const char dsChatLieu[3][25] = {"tu ne", "chu ne", "doan ne"};
    const char tenHienThi[3][25] = {"Tu Ne", "Chu Ne", "Doan Ne"};
    int demCL[3] = {0, 0, 0};
    int demKhac = 0;
    char matThuong[25];

    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].status, TT_SAN_SANG) == 0)
            demSanSang++;
        else if (strcmp(arr[i].status, TT_DANG_DUNG) == 0)
            demDangDung++;
        else
            demBaoTri++;

        tongLuotDung += arr[i].usageCount;
        tongGia += arr[i].price;
        if (arr[i].price > arr[iDat].price)
            iDat = i;
        if (arr[i].price < arr[iRe].price)
            iRe = i;
        if (arr[i].usageCount > arr[iNhieu].usageCount)
            iNhieu = i;

        chuyenThuong(arr[i].material, matThuong);
        int timThay = 0;
        for (int k = 0; k < 3; k++) {
            if (strcmp(matThuong, dsChatLieu[k]) == 0) {
                demCL[k]++;
                timThay = 1;
            }
        }
        if (timThay == 0)
            demKhac++;
    }

    printf("\n1. Theo trang thai:\n");
    printf("   - San sang : %d am\n", demSanSang);
    printf("   - Dang dung: %d am\n", demDangDung);
    printf("   - Bao tri  : %d am\n", demBaoTri);

    printf("\n2. Theo chat lieu:\n");
    for (int k = 0; k < 3; k++)
        printf("   - %-8s: %d am\n", tenHienThi[k], demCL[k]);
    printf("   - %-8s: %d am\n", "Khac", demKhac);

    printf("\n3. Ve gia:\n");
    printf("   - Gia trung binh: %.0f VND\n", tongGia / n);
    printf("   - Am dat nhat   : %s (%.0f VND)\n", arr[iDat].name, arr[iDat].price);
    printf("   - Am re nhat    : %s (%.0f VND)\n", arr[iRe].name, arr[iRe].price);

    printf("\n4. Ve su dung:\n");
    printf("   - Tong so lan da dung (tat ca am): %d\n", tongLuotDung);
    printf("   - Am duoc dung nhieu nhat        : %s (%d lan)\n",
           arr[iNhieu].name, arr[iNhieu].usageCount);

    printf("\n5. Doanh thu tich luy:\n");
    printf("   - So luot cho thue: %d\n", soLuotThue);
    printf("   - Tong doanh thu  : %.0f VND\n", doanhThu);
}

/* ========================= PHAN 7: MENU VA HAM MAIN ========================= */

void hienMenu() {
    printf("\n==========================================\n");
    printf("         QUAN LY PHONG TRA TU SA\n");
    printf("==========================================\n");
    printf("  1. Xem danh sach am tra\n");
    printf("  2. Them am moi\n");
    printf("  3. Tim kiem am (co loc theo so nguoi uong)\n");
    printf("  4. Sua thong tin am\n");
    printf("  5. Xoa am\n");
    printf("  6. Cho thue am (tu van dung tich)\n");
    printf("  7. Nhan tra am\n");
    printf("  8. Hoan tat bao tri\n");
    printf("  9. Sap xep danh sach\n");
    printf(" 10. Thong ke va doanh thu\n");
    printf("  0. Luu & Thoat\n");
    printf("------------------------------------------\n");
}

int main() {
    TeaPot arr[MAX];          // Mang tinh toi da 100 chiec am
    int n = 0;                // So am thuc te dang co
    double doanhThu = 0;      // Tong doanh thu
    int soLuotThue = 0;       // So luot cho thue

    // Neu doc file thanh cong thi lay du lieu cu, neu chua co file thi nap 8 am mau
    if (docFile(arr, n, doanhThu, soLuotThue) == 1) {
        printf("Chao mung den voi Phong Tra Tu Sa! (Da tai %d am tu file %s)\n", n, TEN_FILE);
    } else {
        khoiTaoDuLieu(arr, n);
        luuFile(arr, n, doanhThu, soLuotThue);
        printf("Chao mung den voi Phong Tra Tu Sa! (Da khoi tao %d am mau)\n", n);
    }

    int chon;
    do {
        hienMenu();
        chon = nhapSoNguyen("Nhap lua chon cua ban (0-10): ", 0, 10);
        switch (chon) {
            case 1:
                inDanhSach(arr, n);
                break;
            case 2:
                themAm(arr, n);
                luuFile(arr, n, doanhThu, soLuotThue);
                break;
            case 3:
                timKiem(arr, n);
                break;
            case 4:
                suaAm(arr, n);
                luuFile(arr, n, doanhThu, soLuotThue);
                break;
            case 5:
                xoaAm(arr, n);
                luuFile(arr, n, doanhThu, soLuotThue);
                break;
            case 6:
                choThueAm(arr, n, doanhThu, soLuotThue);
                luuFile(arr, n, doanhThu, soLuotThue);
                break;
            case 7:
                nhanTraAm(arr, n);
                luuFile(arr, n, doanhThu, soLuotThue);
                break;
            case 8:
                hoanTatBaoTri(arr, n);
                luuFile(arr, n, doanhThu, soLuotThue);
                break;
            case 9:
                sapXep(arr, n);
                break;
            case 10:
                thongKe(arr, n, doanhThu, soLuotThue);
                break;
            case 0:
                luuFile(arr, n, doanhThu, soLuotThue);
                printf("\nDa luu du lieu vao file %s. Cam on va hen gap lai!\n", TEN_FILE);
                break;
        }
    } while (chon != 0);

    return 0;
}