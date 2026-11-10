/*
 * ====================================================================
 *  QUẢN LÝ PHÒNG TRÀ TỬ SA - Lập trình C/C++ cơ bản (Sinh viên năm nhất)
 * --------------------------------------------------------------------
 *  Kiến thức sử dụng:
 *   - Cấu trúc struct TeaPot + mảng tĩnh 1 chiều arr[100] + biến đếm n.
 *   - Chuỗi ký tự C char[] với <string.h>: strcpy, strcmp, strstr, strlen.
 *   - Xuất bảng Tiếng Việt UTF-8 căn cột chuẩn bằng printf (<stdio.h>).
 *   - Nhập dữ liệu an toàn bằng cin / cin.getline + cin.ignore.
 *   - Truyền tham chiếu (int &n, double &doanhThu) của C++ cơ bản.
 *   - Đọc / Ghi file nhị phân bằng FILE*, fopen, fread, fwrite.
 * ====================================================================
 */

#include <stdio.h>    // printf, fflush, FILE, fopen, fread, fwrite, fclose
#include <string.h>   // strcpy, strcmp, strstr, strlen
#include <stdlib.h>   // exit
#include <iostream>   // cin, cin.getline, cin.ignore
#ifdef _WIN32
#include <windows.h>  // SetConsoleOutputCP, SetConsoleCP (Hiển thị Tiếng Việt UTF-8)
#endif
using namespace std;

#define MAX 100                      // Số ấm tối đa trong phòng trà
#define DO_RONG 78                   // Độ rộng bảng vừa khít khung Terminal VS Code
#define MUC_BAO_TRI 10               // Cứ 10 lần dùng thì khi trả ấm chuyển sang Bảo trì
#define TEN_FILE "phongtra_tusa.dat" // File lưu dữ liệu nhị phân

// Ba trạng thái hợp lệ của một ấm trà
#define TT_SAN_SANG  "Sẵn sàng"
#define TT_DANG_DUNG "Đang dùng"
#define TT_BAO_TRI   "Bảo trì"

struct TeaPot {
    int id;                 // Mã ấm (1001, 1002...)
    char name[50];          // Tên dáng ấm (Tây Thi, Thạch Biều...) - tăng size cho UTF-8
    char material[40];      // Chất liệu đất (Tử Nê, Chu Nê, Đại Hồng Pao...)
    int capacity;           // Dung tích (ml)
    double price;           // Giá trị ấm / Giá phục vụ (VNĐ)
    char status[30];        // Trạng thái ("Sẵn sàng", "Đang dùng", "Bảo trì")
    int usageCount;         // Số lần đã sử dụng
};

/* ================= PHẦN 1: HÀM TIỆN ÍCH NHẬP / XUẤT ================= */

// Hết dữ liệu nhập (Ctrl+Z / Ctrl+D) thì thoát, tránh lặp vô hạn
void kiemTraHetDuLieu() {
    if (cin.eof()) {
        printf("\nKhông còn dữ liệu nhập. Thoát chương trình.\n");
        exit(0);
    }
}

// Nhập 1 số nguyên trong khoảng [nho, lon]. Nhập sai thì bắt nhập lại.
int nhapSoNguyen(const char thongBao[], int nho, int lon) {
    int x;
    while (1) {
        printf("%s", thongBao);
        fflush(stdout);
        cin >> x;
        kiemTraHetDuLieu();
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            printf("  >> Lỗi: Bạn phải nhập một SỐ nguyên!\n");
            continue;
        }
        cin.ignore(10000, '\n');
        if (x < nho || x > lon) {
            printf("  >> Lỗi: Giá trị phải từ %d đến %d!\n", nho, lon);
            continue;
        }
        return x;
    }
}

// Nhập 1 số thực (double) trong khoảng [nho, lon]
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
            printf("  >> Lỗi: Bạn phải nhập một SỐ!\n");
            continue;
        }
        cin.ignore(10000, '\n');
        if (x < nho || x > lon) {
            printf("  >> Lỗi: Giá trị phải từ %.0f đến %.0f!\n", nho, lon);
            continue;
        }
        return x;
    }
}

// Nhập 1 chuỗi có khoảng trắng (không để trống)
void nhapChuoi(const char thongBao[], char s[], int doDai) {
    while (1) {
        printf("%s", thongBao);
        fflush(stdout);
        cin.getline(s, doDai);
        kiemTraHetDuLieu();
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            printf("  >> Lỗi: Chuỗi quá dài (tối đa %d byte)!\n", doDai - 1);
            continue;
        }
        if (strlen(s) == 0) {
            printf("  >> Lỗi: Không được để trống!\n");
            continue;
        }
        return;
    }
}

// Chuyển các chữ cái tiếng Anh A-Z thành a-z để hỗ trợ tìm kiếm
void chuyenThuong(const char nguon[], char dich[]) {
    int i = 0;
    while (nguon[i] != '\0') {
        if (nguon[i] >= 'A' && nguon[i] <= 'Z')
            dich[i] = nguon[i] + 32;
        else
            dich[i] = nguon[i];
        i++;
    }
    dich[i] = '\0';
}

// Đếm số ký tự hiển thị thực tế của chuỗi Tiếng Việt UTF-8
// (Giúp bảng không bị lệch cột khi chữ có dấu chiếm 2-3 byte)
int demKyTuUTF8(const char s[]) {
    int dem = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        // Trong UTF-8, các byte phụ đi kèm dấu có dạng nhị phân 10xxxxxx (0x80)
        if ((s[i] & 0xC0) != 0x80)
            dem++;
    }
    return dem;
}

// In chuỗi tiếng Việt có dấu và bù đúng số khoảng trắng để thẳng cột
void inCotUTF8(const char s[], int doRongCot) {
    printf("%s", s);
    int soKhoangTrang = doRongCot - demKyTuUTF8(s);
    for (int i = 0; i < soKhoangTrang; i++)
        printf(" ");
}

// Cho chọn 1 trong 3 trạng thái, ghi kết quả vào chuỗi 'dich'
void chonTrangThai(char dich[]) {
    printf("   1. %s\n   2. %s\n   3. %s\n", TT_SAN_SANG, TT_DANG_DUNG, TT_BAO_TRI);
    int c = nhapSoNguyen("   Chọn trạng thái (1-3): ", 1, 3);
    if (c == 1)
        strcpy(dich, TT_SAN_SANG);
    else if (c == 2)
        strcpy(dich, TT_DANG_DUNG);
    else
        strcpy(dich, TT_BAO_TRI);
}

// Cho chọn nhanh 1 trong 6 chất liệu đất Tử Sa phổ biến
void chonChatLieu(char dich[]) {
    printf("   Chọn chất liệu đất Tử Sa:\n");
    printf("   1. Tử Nê          2. Chu Nê         3. Đoàn Nê\n");
    printf("   4. Đại Hồng Pao   5. Hắc Nê         6. Lục Nê\n");
    printf("   7. Tự nhập chất liệu khác\n");
    int c = nhapSoNguyen("   Lựa chọn của bạn (1-7): ", 1, 7);
    if (c == 1) strcpy(dich, "Tử Nê");
    else if (c == 2) strcpy(dich, "Chu Nê");
    else if (c == 3) strcpy(dich, "Đoàn Nê");
    else if (c == 4) strcpy(dich, "Đại Hồng Pao");
    else if (c == 5) strcpy(dich, "Hắc Nê");
    else if (c == 6) strcpy(dich, "Lục Nê");
    else nhapChuoi("   Nhập tên chất liệu: ", dich, 40);
}

// In 1 đường kẻ ngang gồm 'doDai' dấu gạch
void inDuongKe(int doDai) {
    for (int i = 0; i < doDai; i++)
        printf("-");
    printf("\n");
}

// In dòng tiêu đề của bảng (gọn gàng trong 78 ký tự)
void inTieuDe() {
    inDuongKe(DO_RONG);
    printf("%-6s ", "ID");
    inCotUTF8("TÊN DÁNG ẤM", 15);
    inCotUTF8("CHẤT LIỆU", 15);
    inCotUTF8("D.TÍCH(ml)", 11);
    inCotUTF8("GIÁ (VNĐ)", 13);
    inCotUTF8("TRẠNG THÁI", 12);
    printf("%-6s\n", "SỐ LẦN");
    inDuongKe(DO_RONG);
}

// In 1 dòng dữ liệu của 1 chiếc ấm
void inMotAm(TeaPot a) {
    printf("%-6d ", a.id);
    inCotUTF8(a.name, 15);
    inCotUTF8(a.material, 15);
    printf("%-11d %-13.0f ", a.capacity, a.price);
    inCotUTF8(a.status, 12);
    printf("%-6d\n", a.usageCount);
}

// In toàn bộ danh sách ấm trà
void inDanhSach(TeaPot arr[], int n) {
    if (n == 0) {
        printf("\n>> Phòng trà chưa có chiếc ấm nào!\n");
        return;
    }
    printf("\n==================== DANH SÁCH ẤM TRÀ TỬ SA ====================\n");
    inTieuDe();
    for (int i = 0; i < n; i++)
        inMotAm(arr[i]);
    inDuongKe(DO_RONG);
    printf("Tổng cộng: %d ấm trà\n", n);
}

// In các ấm có trạng thái 'trangThai'. Trả về số ấm đã in.
int inAmTheoTrangThai(TeaPot arr[], int n, const char trangThai[]) {
    int dem = 0;
    for (int i = 0; i < n; i++) {
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

/* ============ PHẦN 2: ĐỌC/GHI FILE, KHỞI TẠO & QUẢN LÝ (THÊM / SỬA / XÓA) ============ */

// Ghi toàn bộ danh sách ấm và doanh thu ra file nhị phân
void luuFile(TeaPot arr[], int n, double doanhThu, int soLuotThue) {
    FILE *f = fopen(TEN_FILE, "wb");
    if (f == NULL) {
        printf(">> Lỗi: Không thể mở file %s để ghi!\n", TEN_FILE);
        return;
    }
    fwrite(&n, sizeof(int), 1, f);
    fwrite(&doanhThu, sizeof(double), 1, f);
    fwrite(&soLuotThue, sizeof(int), 1, f);
    if (n > 0) {
        fwrite(arr, sizeof(TeaPot), n, f);
    }
    fclose(f);
}

// Đọc dữ liệu từ file nhị phân lên mảng
int docFile(TeaPot arr[], int &n, double &doanhThu, int &soLuotThue) {
    FILE *f = fopen(TEN_FILE, "rb");
    if (f == NULL) {
        return 0;
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

// Thêm 1 ấm có sẵn thông tin vào mảng
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

// Nạp sẵn 15 dáng ấm Tử Sa kinh điển với GIÁ THỊ TRƯỜNG THỰC TẾ (1.5 triệu - 12.5 triệu VNĐ)
void khoiTaoDuLieu(TeaPot arr[], int &n) {
    themAmMau(arr, n, 1001, "Tây Thi",     "Tử Nê",        150,  1800000, TT_SAN_SANG,  4);
    themAmMau(arr, n, 1002, "Thạch Biều",  "Chu Nê",       220,  3500000, TT_DANG_DUNG, 7);
    themAmMau(arr, n, 1003, "Phúc Đăng",   "Đoàn Nê",      180,  2200000, TT_SAN_SANG,  3);
    themAmMau(arr, n, 1004, "Bán Nguyệt",  "Tử Nê",        120,  1500000, TT_BAO_TRI,  10);
    themAmMau(arr, n, 1005, "Dương Tiễn",  "Chu Nê",       250,  4800000, TT_SAN_SANG,  8);
    themAmMau(arr, n, 1006, "Long Đán",    "Đoàn Nê",      300,  5500000, TT_SAN_SANG,  2);
    themAmMau(arr, n, 1007, "Cung Xuân",   "Tử Nê",        160,  8500000, TT_SAN_SANG,  5);
    themAmMau(arr, n, 1008, "Báo Xuân",    "Đại Hồng Pao", 360, 12500000, TT_SAN_SANG,  6);
    themAmMau(arr, n, 1009, "Đức Chung",   "Tử Nê",        200,  3200000, TT_SAN_SANG,  9);
    themAmMau(arr, n, 1010, "Thủy Bình",   "Chu Nê",       140,  2600000, TT_SAN_SANG,  5);
    themAmMau(arr, n, 1011, "Phỏng Cổ",    "Hắc Nê",       240,  4200000, TT_DANG_DUNG, 3);
    themAmMau(arr, n, 1012, "Trúc Đoạn",   "Lục Nê",       210,  3900000, TT_SAN_SANG,  4);
    themAmMau(arr, n, 1013, "Tỉnh Lan",    "Đoàn Nê",      280,  5100000, TT_SAN_SANG,  1);
    themAmMau(arr, n, 1014, "Tần Quyền",   "Tử Nê",        320,  6400000, TT_SAN_SANG,  7);
    themAmMau(arr, n, 1015, "Hợp Hoan",    "Đại Hồng Pao", 190,  9800000, TT_SAN_SANG,  2);
}

// Tìm ấm theo mã. Trả về vị trí trong mảng, không thấy thì trả về -1
int timTheoId(TeaPot arr[], int n, int id) {
    for (int i = 0; i < n; i++) {
        if (arr[i].id == id)
            return i;
    }
    return -1;
}

// Hỏi người dùng mã ấm, trả về vị trí trong mảng (-1 nếu không có)
int nhapVaTimId(TeaPot arr[], int n) {
    int id = nhapSoNguyen("Nhập mã ấm: ", 1, 999999);
    int vt = timTheoId(arr, n, id);
    if (vt == -1)
        printf(">> Không tìm thấy ấm có mã %d!\n", id);
    return vt;
}

// Mã mới = mã lớn nhất hiện có + 1
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

// Chức năng 2: Thêm ấm mới
void themAm(TeaPot arr[], int &n) {
    printf("\n===== THÊM ẤM MỚI =====\n");
    if (n >= MAX) {
        printf(">> Phòng trà đã đầy (%d ấm), không thể thêm nữa!\n", MAX);
        return;
    }
    TeaPot a;
    a.id = taoIdMoi(arr, n);
    printf("Mã ấm mới (tự động tạo): %d\n", a.id);
    nhapChuoi("Nhập tên dáng ấm (VD: Tây Thi, Thủy Bình...): ", a.name, 50);
    chonChatLieu(a.material);
    a.capacity = nhapSoNguyen("Nhập dung tích (ml, từ 50 đến 1000): ", 50, 1000);
    a.price = nhapSoThuc("Nhập giá trị ấm (VNĐ, từ 500.000 đến 100.000.000): ", 500000, 100000000);
    strcpy(a.status, TT_SAN_SANG);
    a.usageCount = 0;
    arr[n] = a;
    n++;
    printf(">> Đã thêm thành công ấm \"%s\" (mã %d)!\n", a.name, a.id);
}

// Chức năng 4: Sửa thông tin 1 chiếc ấm
void suaAm(TeaPot arr[], int n) {
    printf("\n===== SỬA THÔNG TIN ẤM =====\n");
    if (n == 0) {
        printf(">> Phòng trà chưa có ấm nào!\n");
        return;
    }
    int vt = nhapVaTimId(arr, n);
    if (vt == -1)
        return;

    int chon;
    do {
        printf("\nThông tin hiện tại:\n");
        inTieuDe();
        inMotAm(arr[vt]);
        inDuongKe(DO_RONG);
        printf("  1. Tên dáng ấm     2. Chất liệu    3. Dung tích\n");
        printf("  4. Giá trị (VNĐ)   5. Trạng thái   0. Quay lại\n");
        chon = nhapSoNguyen("Chọn mục cần sửa: ", 0, 5);
        switch (chon) {
            case 1: nhapChuoi("Tên mới: ", arr[vt].name, 50); break;
            case 2: chonChatLieu(arr[vt].material); break;
            case 3: arr[vt].capacity = nhapSoNguyen("Dung tích mới (ml): ", 50, 1000); break;
            case 4: arr[vt].price = nhapSoThuc("Giá mới (VNĐ): ", 500000, 100000000); break;
            case 5: chonTrangThai(arr[vt].status); break;
        }
        if (chon != 0)
            printf(">> Đã cập nhật!\n");
    } while (chon != 0);
}

// Chức năng 5: Xóa ấm (dồn các phần tử phía sau lên 1 ô)
void xoaAm(TeaPot arr[], int &n) {
    printf("\n===== XÓA ẤM =====\n");
    if (n == 0) {
        printf(">> Phòng trà chưa có ấm nào!\n");
        return;
    }
    int vt = nhapVaTimId(arr, n);
    if (vt == -1)
        return;
    if (strcmp(arr[vt].status, TT_DANG_DUNG) == 0) {
        printf(">> Ấm đang có khách sử dụng, không thể xóa!\n");
        return;
    }
    inTieuDe();
    inMotAm(arr[vt]);
    inDuongKe(DO_RONG);
    if (nhapSoNguyen("Bạn chắc chắn muốn xóa? (1: Có, 0: Không): ", 0, 1) == 0) {
        printf(">> Đã hủy thao tác xóa.\n");
        return;
    }
    for (int i = vt; i < n - 1; i++)
        arr[i] = arr[i + 1];
    n--;
    printf(">> Đã xóa ấm thành công!\n");
}

/* ============================ PHẦN 3: TÌM KIẾM ============================ */

// Tìm theo tên dáng ấm hoặc chất liệu
void timTheoChuoi(TeaPot arr[], int n, int theoTen) {
    char tuKhoa[50], khoaThuong[50], truongThuong[50];
    if (theoTen == 1)
        nhapChuoi("Nhập từ khóa trong tên ấm: ", tuKhoa, 50);
    else
        chonChatLieu(tuKhoa);
    chuyenThuong(tuKhoa, khoaThuong);

    int dem = 0;
    for (int i = 0; i < n; i++) {
        int khop;
        if (theoTen == 1) {
            chuyenThuong(arr[i].name, truongThuong);
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
        printf(">> Không tìm thấy ấm nào khớp với \"%s\"!\n", tuKhoa);
    else {
        inDuongKe(DO_RONG);
        printf("Tìm thấy %d ấm.\n", dem);
    }
}

// Tìm và lọc ấm trà theo số người uống (Công thức: searchVol = numPeople * 60 + 30)
void timTheoSoNguoiUong(TeaPot arr[], int n) {
    int soNguoi = nhapSoNguyen("Nhập số người uống trà (1-20): ", 1, 20);
    int searchVol = soNguoi * 60 + 30;

    printf("\n---------------- THÔNG TIN TÍNH TOÁN DUNG TÍCH ----------------\n");
    printf(" - Số người uống            : %d người\n", soNguoi);
    printf(" - Dung tích chén trà       : %d x 60 ml = %d ml\n", soNguoi, soNguoi * 60);
    printf(" - Dung tích trà nở trong ấm: 30 ml\n");
    printf(" => Dung tích ấm tối thiểu cần tìm (capacity >= %d ml)\n", searchVol);

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
        printf(">> Không có ấm nào có dung tích >= %d ml!\n", searchVol);
    else {
        inDuongKe(DO_RONG);
        printf("=> Tìm thấy tổng cộng %d ấm trà phù hợp.\n", dem);
    }
}

// Chức năng 3: Menu con tìm kiếm
void timKiem(TeaPot arr[], int n) {
    printf("\n===== TÌM KIẾM ẤM TRÀ =====\n");
    if (n == 0) {
        printf(">> Phòng trà chưa có ấm nào!\n");
        return;
    }
    printf("  1. Theo mã ấm\n");
    printf("  2. Theo tên dáng ấm\n");
    printf("  3. Theo chất liệu đất\n");
    printf("  4. Theo trạng thái\n");
    printf("  5. Theo số người uống (tự động tính dung tích: số người * 60 + 30)\n");
    int chon = nhapSoNguyen("Chọn cách tìm (1-5): ", 1, 5);
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
        char tt[30];
        chonTrangThai(tt);
        if (inAmTheoTrangThai(arr, n, tt) == 0)
            printf(">> Không có ấm nào ở trạng thái \"%s\"!\n", tt);
    } else {
        timTheoSoNguoiUong(arr, n);
    }
}

/* ======================= PHẦN 4: SẮP XẾP (ĐỔI CHỖ TRỰC TIẾP) ======================= */

// Chức năng 9: Sắp xếp danh sách theo nhiều tiêu chí (Interchange Sort)
void sapXep(TeaPot arr[], int n) {
    printf("\n===== SẮP XẾP DANH SÁCH =====\n");
    if (n < 2) {
        printf(">> Cần ít nhất 2 ấm để sắp xếp!\n");
        return;
    }
    printf("  1. Giá tăng dần           2. Giá giảm dần\n");
    printf("  3. Số lần dùng giảm dần   4. Tên A -> Z\n");
    printf("  5. Dung tích tăng dần\n");
    int kieu = nhapSoNguyen("Chọn cách sắp xếp (1-5): ", 1, 5);

    char tenI[50], tenJ[50];
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            int doiCho = 0;
            if (kieu == 1 && arr[j].price < arr[i].price)
                doiCho = 1;
            else if (kieu == 2 && arr[j].price > arr[i].price)
                doiCho = 1;
            else if (kieu == 3 && arr[j].usageCount > arr[i].usageCount)
                doiCho = 1;
            else if (kieu == 4) {
                chuyenThuong(arr[i].name, tenI);
                chuyenThuong(arr[j].name, tenJ);
                if (strcmp(tenJ, tenI) < 0)
                    doiCho = 1;
            }
            else if (kieu == 5 && arr[j].capacity < arr[i].capacity)
                doiCho = 1;

            if (doiCho == 1) {
                TeaPot tam = arr[i];
                arr[i] = arr[j];
                arr[j] = tam;
            }
        }
    }
    printf(">> Đã sắp xếp xong!\n");
    inDanhSach(arr, n);
}

/* ============ PHẦN 5: NGHIỆP VỤ PHÒNG TRÀ (THUÊ / TRẢ / BẢO TRÌ) ============ */

// Hiện các ấm đang ở 'trangThai', cho nhập mã và kiểm tra trạng thái
int chonAmTheoTrangThai(TeaPot arr[], int n, const char trangThai[]) {
    printf("Các ấm đang \"%s\":\n", trangThai);
    if (inAmTheoTrangThai(arr, n, trangThai) == 0) {
        printf(">> Hiện không có ấm nào ở trạng thái này!\n");
        return -1;
    }
    int vt = nhapVaTimId(arr, n);
    if (vt == -1)
        return -1;
    if (strcmp(arr[vt].status, trangThai) != 0) {
        printf(">> Ấm này đang \"%s\", không phù hợp!\n", arr[vt].status);
        return -1;
    }
    return vt;
}

// Chức năng 6: Cho thuê ấm / Phục vụ bàn trà
void choThueAm(TeaPot arr[], int n, double &doanhThu, int &soLuotThue) {
    printf("\n===== CHO THUÊ ẤM & TƯ VẤN BÀN TRÀ =====\n");
    int soNguoi = nhapSoNguyen("Nhập số người uống trà (nhập 0 nếu xem tất cả ấm Sẵn sàng): ", 0, 20);
    int dungTichCan = 0;

    if (soNguoi > 0) {
        dungTichCan = soNguoi * 60 + 30;
        printf("=> Gợi ý: %d người x 60ml + 30ml trà nở = Cần ấm tối thiểu %d ml.\n\n",
               soNguoi, dungTichCan);
    }

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
        printf(">> Không có ấm nào SẴN SÀNG đáp ứng dung tích >= %d ml!\n", dungTichCan);
        return;
    }
    inDuongKe(DO_RONG);

    int vt = nhapVaTimId(arr, n);
    if (vt == -1)
        return;
    if (strcmp(arr[vt].status, TT_SAN_SANG) != 0) {
        printf(">> Ấm này đang \"%s\", không thể cho thuê!\n", arr[vt].status);
        return;
    }
    if (arr[vt].capacity < dungTichCan) {
        printf(">> Cảnh báo: Ấm này (%d ml) nhỏ hơn dung tích gợi ý (%d ml)!\n",
               arr[vt].capacity, dungTichCan);
        if (nhapSoNguyen("Khách vẫn muốn chọn ấm này? (1: Có, 0: Hủy): ", 0, 1) == 0)
            return;
    }

    strcpy(arr[vt].status, TT_DANG_DUNG);
    arr[vt].usageCount++;
    doanhThu += arr[vt].price;
    soLuotThue++;

    printf("\n--------- PHIẾU PHỤC VỤ ---------\n");
    printf("Ấm        : %s (mã %d)\n", arr[vt].name, arr[vt].id);
    printf("Chất liệu : %s - %d ml\n", arr[vt].material, arr[vt].capacity);
    printf("Giá trị   : %.0f VNĐ\n", arr[vt].price);
    printf("---------------------------------\n");
    printf(">> Giao ấm thành công! (Lần dùng thứ %d)\n", arr[vt].usageCount);
}

// Chức năng 7: Khách trả ấm
void nhanTraAm(TeaPot arr[], int n) {
    printf("\n===== NHẬN TRẢ ẤM =====\n");
    int vt = chonAmTheoTrangThai(arr, n, TT_DANG_DUNG);
    if (vt == -1)
        return;
    if (arr[vt].usageCount % MUC_BAO_TRI == 0) {
        strcpy(arr[vt].status, TT_BAO_TRI);
        printf(">> Đã nhận lại ấm. Ấm đã dùng %d lần nên chuyển sang BẢO TRÌ (dưỡng ấm).\n",
               arr[vt].usageCount);
    } else {
        strcpy(arr[vt].status, TT_SAN_SANG);
        printf(">> Đã nhận lại ấm. Ấm trở về trạng thái SẴN SÀNG.\n");
    }
}

// Chức năng 8: Bảo trì xong, đưa ấm về trạng thái sẵn sàng
void hoanTatBaoTri(TeaPot arr[], int n) {
    printf("\n===== HOÀN TẤT BẢO TRÌ =====\n");
    int vt = chonAmTheoTrangThai(arr, n, TT_BAO_TRI);
    if (vt == -1)
        return;
    strcpy(arr[vt].status, TT_SAN_SANG);
    printf(">> Ấm \"%s\" đã dưỡng/bảo trì xong và sẵn sàng phục vụ khách!\n", arr[vt].name);
}

/* ============================ PHẦN 6: THỐNG KÊ ============================ */

// Chức năng 10: Thống kê tổng hợp
void thongKe(TeaPot arr[], int n, double doanhThu, int soLuotThue) {
    printf("\n===== THỐNG KÊ PHÒNG TRÀ TỬ SA =====\n");
    if (n == 0) {
        printf(">> Phòng trà chưa có ấm nào để thống kê!\n");
        return;
    }
    int demSanSang = 0, demDangDung = 0, demBaoTri = 0;
    int tongLuotDung = 0;
    double tongGia = 0;
    int iDat = 0, iRe = 0, iNhieu = 0;

    // 6 chất liệu đất Tử Sa phổ biến
    const char dsChatLieu[6][40] = {"Tử Nê", "Chu Nê", "Đoàn Nê", "Đại Hồng Pao", "Hắc Nê", "Lục Nê"};
    int demCL[6] = {0, 0, 0, 0, 0, 0};
    int demKhac = 0;

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

        int timThay = 0;
        for (int k = 0; k < 6; k++) {
            if (strcmp(arr[i].material, dsChatLieu[k]) == 0) {
                demCL[k]++;
                timThay = 1;
            }
        }
        if (timThay == 0)
            demKhac++;
    }

    printf("\n1. Theo trạng thái:\n");
    printf("   - Sẵn sàng : %d ấm\n", demSanSang);
    printf("   - Đang dùng: %d ấm\n", demDangDung);
    printf("   - Bảo trì  : %d ấm\n", demBaoTri);

    printf("\n2. Theo chất liệu đất:\n");
    for (int k = 0; k < 6; k++) {
        printf("   - ");
        inCotUTF8(dsChatLieu[k], 14);
        printf(": %d ấm\n", demCL[k]);
    }
    if (demKhac > 0) {
        printf("   - ");
        inCotUTF8("Khác", 14);
        printf(": %d ấm\n", demKhac);
    }

    printf("\n3. Về giá trị ấm:\n");
    printf("   - Tổng giá trị kho ấm: %.0f VNĐ\n", tongGia);
    printf("   - Giá trung bình     : %.0f VNĐ\n", tongGia / n);
    printf("   - Ấm đắt giá nhất    : %s - %s (%.0f VNĐ)\n", arr[iDat].name, arr[iDat].material, arr[iDat].price);
    printf("   - Ấm giá mềm nhất    : %s - %s (%.0f VNĐ)\n", arr[iRe].name, arr[iRe].material, arr[iRe].price);

    printf("\n4. Về tần suất sử dụng:\n");
    printf("   - Tổng số lượt đã dùng (tất cả ấm): %d lần\n", tongLuotDung);
    printf("   - Ấm được khách chuộng nhất       : %s (%d lần)\n",
           arr[iNhieu].name, arr[iNhieu].usageCount);

    printf("\n5. Doanh thu tích lũy:\n");
    printf("   - Số lượt giao ấm : %d lượt\n", soLuotThue);
    printf("   - Tổng giá trị giao dịch: %.0f VNĐ\n", doanhThu);
}

/* ========================= PHẦN 7: MENU VÀ HÀM MAIN ========================= */

void hienMenu() {
    printf("\n============================================\n");
    printf("          QUẢN LÝ PHÒNG TRÀ TỬ SA\n");
    printf("============================================\n");
    printf("  1. Xem danh sách ấm trà (15+ dáng ấm)\n");
    printf("  2. Thêm ấm mới\n");
    printf("  3. Tìm kiếm ấm (có lọc theo số người uống)\n");
    printf("  4. Sửa thông tin ấm\n");
    printf("  5. Xóa ấm\n");
    printf("  6. Cho thuê ấm (tư vấn dung tích)\n");
    printf("  7. Nhận trả ấm\n");
    printf("  8. Hoàn tất bảo trì\n");
    printf("  9. Sắp xếp danh sách\n");
    printf(" 10. Thống kê và doanh thu\n");
    printf(" 11. Khôi phục 15 ấm mẫu mặc định\n");
    printf("  0. Lưu & Thoát\n");
    printf("--------------------------------------------\n");
}

int main() {
#ifdef _WIN32
    // Bật chế độ hiển thị và nhập liệu Tiếng Việt UTF-8 trên Windows Console
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    TeaPot arr[MAX];          // Mảng tĩnh tối đa 100 chiếc ấm
    int n = 0;                // Số ấm thực tế đang có
    double doanhThu = 0;      // Tổng doanh thu
    int soLuotThue = 0;       // Số lượt cho thuê

    // Nếu đọc file thành công thì lấy dữ liệu cũ, nếu chưa có file thì nạp 15 ấm mẫu
    if (docFile(arr, n, doanhThu, soLuotThue) == 1) {
        printf("Chào mừng đến với Phòng Trà Tử Sa! (Đã tải %d ấm từ file %s)\n", n, TEN_FILE);
    } else {
        khoiTaoDuLieu(arr, n);
        luuFile(arr, n, doanhThu, soLuotThue);
        printf("Chào mừng đến với Phòng Trà Tử Sa! (Đã khởi tạo %d ấm mẫu giá thị trường)\n", n);
    }

    int chon;
    do {
        hienMenu();
        chon = nhapSoNguyen("Nhập lựa chọn của bạn (0-11): ", 0, 11);
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
            case 11:
                if (nhapSoNguyen("Bạn muốn đặt lại danh sách về 15 ấm mẫu ban đầu? (1: Có, 0: Hủy): ", 0, 1) == 1) {
                    n = 0;
                    doanhThu = 0;
                    soLuotThue = 0;
                    khoiTaoDuLieu(arr, n);
                    luuFile(arr, n, doanhThu, soLuotThue);
                    printf(">> Đã khôi phục 15 ấm Tử Sa mẫu thành công!\n");
                }
                break;
            case 0:
                luuFile(arr, n, doanhThu, soLuotThue);
                printf("\nĐã lưu dữ liệu vào file %s. Cảm ơn và hẹn gặp lại!\n", TEN_FILE);
                break;
        }
    } while (chon != 0);

    return 0;
}
