# **Giới thiệu dự án**

**Tên dự án:** Hệ Thống đăng nhập và đăng ký tài khoản và quản lý ví điểm thưởng

**Mục tiêu:**

Xây dựng hệ thống đăng nhập, đăng ký tài khoản người dùng. Thông tin xác thực người dùng gồm tên người dùng và mật khẩu được yêu cầu từ người dùng. Nếu người dùng đăng ký thành công thì thông tin tài khoản sẽ được lưu lại trong một bản ghi.

Ví điểm thưởng cho phép người dùng chuyển điểm, xem lịch sử giao dịch.

---

# **Giới thiệu thành viên & Phân công công việc**

1. **Doãn Tuấn Anh - K24DTCN569**
    - Xây dựng lớp **User**: quản lý thông tin người dùng (username, password, họ tên, trạng thái, quyền hạn).
    - Xây dựng lớp **Wallet**: quản lý số dư điểm thưởng, các thao tác cộng/trừ điểm, lịch sử giao dịch.
    - Hàm **Hash Password** (trong lớp Utils): mã hóa mật khẩu khi lưu trữ, xác thực đăng nhập.
    - Hàm **Auto Gen Password** (trong lớp Utils): tự động sinh mật khẩu ngẫu nhiên khi tạo tài khoản mới.
2. **Phạm Ngọc Anh - K25DTCN002**
    - Xây dựng lớp **Transaction**: quản lý các giao dịch chuyển điểm, lưu lịch sử giao dịch.
    - Xây dựng lớp **Pending Change**: quản lý các thay đổi thông tin tài khoản cần xác thực (pending), hỗ trợ rollback khi cần thiết.
    - Phát triển lớp **OTP**: sinh mã OTP, xác thực OTP khi thực hiện các thao tác quan trọng (đăng nhập, chuyển điểm, đổi thông tin).
3. **Phùng Thị Thảo - K24DTCN639**
    - Xây dựng lớp **User Manager**: xử lý logic tổng thể các tính năng:
        - Đăng ký, đăng nhập tài khoản.
        - Quản lý ví điểm thưởng.
        - Thực hiện giao dịch chuyển điểm.
        - Kết nối, phối hợp các lớp User, Wallet, Transaction, Pending Change, OTP để đảm bảo luồng hoạt động của hệ thống.
4. **Nguyễn Hồng Sơn - K24DTCN624**
    - Xây dựng lớp **Handles + Main (menu)**:
        - Thiết kế giao diện hiển thị (dòng lệnh), xây dựng menu chức năng.
        - Xử lý logic nhập/xuất dữ liệu đầu vào từ người dùng.
        - Kết nối các chức năng: đăng ký, đăng nhập, quản lý ví, thực hiện giao dịch.
        - Xây dựng cơ chế sao lưu/khôi phục dữ liệu và tích hợp vào giao diện menu.
    - Viết **README.md.**

---

# **Phân tích & Đặc tả chức năng**

## **1. Đăng ký tài khoản**

- Nhập username, password, họ tên.
- Chỉ được chọn quyền (người dùng/quản lý) trong trường hợp tạo hộ tài khoản từ tài khoản quản lý, còn lại mặc định sẽ là người dùng thường.
- Kiểm tra trùng username.
- Lưu thông tin vào file.

## **2. Đăng nhập**

- Nhập username, password.
- Xác thực OTP (giả lập gửi OTP).
- Kiểm tra trạng thái tài khoản (yêu cầu đổi mật khẩu lần đầu).

## **3. Quản lý tài khoản**

- Đổi mật khẩu, đổi họ tên (có xác thực OTP).
- Xem thông tin tài khoản, ví, lịch sử giao dịch.
- Chuyển điểm.

## **4. Chuyển điểm**

- Nhập username người nhận, số điểm, xác thực OTP.
- Kiểm tra số dư, cập nhật số dư, lưu giao dịch.

## **5. Quản lý giao dịch**

- Lưu lịch sử giao dịch, rollback giao dịch lỗi.
- Xem lịch sử giao dịch cá nhân.

## **6. Chức năng dành cho người dùng quản lý**

- Tạo tài khoản mới.
- Theo dõi danh sách tài khoản.
- Điều chỉnh thông tin tài khoản khác (qua pending change + OTP).

## **7. Sao lưu & khôi phục dữ liệu (dành cho admin)**

- Tự động backup khi có thay đổi dữ liệu.
- Admin có thể xem, chọn file backup để khôi phục.

## 8. Ví

- Tài khoản ví tổng ban đầu chứa 1000000 điểm:
    - **username**: system_wallet
    - **password**: system_pass
    - Tài khoản này cũng là tài khoản quản lý mặc định của hệ thống, có quyền xem sao lưu và khôi phục dữ liệu.
- Các tài khoản khác khi đăng ký đều có số điểm ban đầu trong ví bằng 0.
- Nếu muốn cấp điểm cho tài khoản mới có thể chuyển điểm từ tài khoản ví tổng sang.

---

# Các tập tin chính

## Thư mục data

- Chứa dữ liệu người dùng, giao dịch, yêu cầu đợi xác nhận thay đổi.
- Gồm các file:
    - **users.txt**: dữ liệu toàn bộ tài khoản người dùng.
    - **transactions.txt**: dữ liệu toàn bộ các giao dịch.
    - **pendingchanges.txt**: dữ liệu các thay đổi cần xác nhận của chủ tài khoản.

## Thư mục backup

- Chứa các tệp tin sao lưu dữ liệu tài khoản người dùng.
- VD: users_1748706545.txt,…

## Thư mục docs

- Chứa tài liệu project.

## Thư mục src

Chứa các tập lệnh chương trình.

### 1. **main.cpp**

- **Chức năng:** Chứa hàm main() và luồng điều khiển chính của chương trình. Quản lý menu, đăng nhập, đăng ký, gọi các hàm xử lý nghiệp vụ.

### 2. **handlers.cpp / handlers.h**

- **Chức năng:** Chứa các hàm xử lý nghiệp vụ (đăng ký, đăng nhập, đổi mật khẩu, chuyển điểm, xem backup, khôi phục backup, v.v).
- **handlers.h** là file khai báo các hàm, **handlers.cpp** là file hiện thực.

### 3. **user.cpp / user.h**

- **Chức năng:** Định nghĩa lớp User (người dùng), các thuộc tính (username, password, fullName, quyền quản lý, v.v) và các phương thức thao tác với người dùng.

### 4. **wallet.cpp / wallet.h**

- **Chức năng:** Định nghĩa lớp Wallet (ví), quản lý số dư, lịch sử giao dịch của từng người dùng.

### 5. **transaction.cpp / transaction.h**

- **Chức năng:** Định nghĩa lớp Transaction (giao dịch), lưu thông tin giao dịch chuyển điểm giữa các ví.

### 6. **usermanager.cpp / usermanager.h**

- **Chức năng:** Quản lý danh sách người dùng, các thao tác thêm/xóa/sửa user, backup, khôi phục dữ liệu, kiểm tra quyền admin, v.v.

### 7. **otp.cpp / otp.h**

- **Chức năng:** Xử lý OTP (One-Time Password) cho các thao tác xác thực (ví dụ: chuyển điểm, xác nhận đăng nhập,…).

### 8. **pendingchange.cpp / pendingchange.h**

- **Chức năng:** Quản lý các thay đổi thông tin đang chờ xác nhận (pending), như đổi mật khẩu, đổi tên.

### 9. **utils.cpp / utils.h**

- **Chức năng:** Chứa các hàm tiện ích dùng chung cho toàn bộ chương trình (ví dụ: hash password, mở file, lấy thời gian hiện tại, xóa bộ đệm nhập liệu,…).

### 10. **sources.txt**

- **Chức năng:** Danh sách các file nguồn của dự án (có thể dùng cho build script hoặc tài liệu).

---

# **Hướng dẫn chạy chương trình**

## **Dịch chương trình**

1. Mở project bằng **Visual Studio Code.**
2. Mở file **main.cpp** trong thư mục **src**.
3. Bấm **RUN C/C++ File**. (sau khi build sẽ chạy luôn, đồng thời có file **main.exe**).

Chương trình được hoàn thiện trên hệ thống:

- Hệ điều hành: Windows 11.
- Trình biên dịch: G++ (MinGW-w64).
- Trình soạn thảo mã nguồn: VS Code.

---

# Cách sử dụng chương trình & các thao tác thực hiện

## 1. Giao diện Menu chính

Khi chạy chương trình, bạn sẽ thấy menu chính như sau:

```
=== He Thong Quan Ly Vi ===
1. Dang ky
2. Dang nhap
3. Thoat
--------------------------
Chon:
```

- **Nhập số 1:** Đăng ký tài khoản mới. Làm theo hướng dẫn để nhập tên đăng nhập, mật khẩu, họ tên, v.v.
- **Nhập số 2:** Đăng nhập vào hệ thống. Sau khi đăng nhập thành công, bạn sẽ vào menu chức năng bên trong.
- **Nhập số 3:** Thoát chương trình.

<aside>
💡

**Lưu ý**: tài khoản quản lý chỉ được đăng ký thông qua 1 tài khoản quản lý khác.

</aside>

```
Chon: 2
=== Dang Nhap ===
Nhap username: nam
Nhan ESC de thoat. Enter de tiep tuc.

```

- Chọn 2 để thực hiện đăng nhập.
- Mỗi 1 thao tác nhập vào sẽ có lựa chọn tiếp tục hoặc thoát tiến trình trở về menu.

```
Chon: 2
=== Dang Nhap ===
Nhap username: nam
Nhan ESC de thoat. Enter de tiep tuc.
Nhap password: nam123
Nhan ESC de thoat. Enter de tiep tuc.
Ma OTP da duoc gui: 025504
Ban yeu cau dang nhap [nam]. Nhap OTP de xac nhan.
Nhap OTP:
```

- Mã OTP sẽ được gửi để xác nhận đăng nhập.

```
=== Menu Chuc Nang ===
1. Thay doi mat khau
2. Thay doi ho ten
3. Xem thong tin tai khoan
4. Xem thong tin vi
5. Xem thong tin giao dich
6. Chuyen diem
7. Dang xuat
--------------------------
Chon:
```

## 2. Menu chức năng bên trong (sau khi đăng nhập thành công)

### Đối với người dùng thường:

```
=== Menu Chuc Nang ===
1. Thay doi mat khau
2. Thay doi ho ten
3. Xem thong tin tai khoan
4. Xem thong tin vi
5. Xem thong tin giao dich
6. Chuyen diem
7. Dang xuat
--------------------------
Chon:
```

### Đối với người dùng quản lý:

```
=== Menu Chuc Nang ===
1. Thay doi mat khau
2. Thay doi ho ten
3. Xem thong tin tai khoan
4. Xem thong tin vi
5. Xem thong tin giao dich
6. Chuyen diem
7. Tao tai khoan moi (Quan ly)
8. Theo doi danh sach (Quan ly)
9. Dieu chinh thong tin tai khoan khac (Quan ly)
10. Dang xuat
--------------------------
Chon:
```

- Có thêm tạo tài khoản mới, theo dõi danh sách tài khoản và điều chỉnh thông tin tài khoản khác.

### Đối với tài khoản admin (system_wallet):

```
=== Menu Chuc Nang ===
1. Thay doi mat khau
2. Thay doi ho ten
3. Xem thong tin tai khoan
4. Xem thong tin vi
5. Xem thong tin giao dich
6. Chuyen diem
7. Tao tai khoan moi (Quan ly)
8. Theo doi danh sach (Quan ly)
9. Dieu chinh thong tin tai khoan khac (Quan ly)
10. Xem va khoi phuc du lieu sao luu (Admin)
11. Dang xuat
--------------------------
Chon:
```

- Có thêm xem và khôi phục dữ liệu.

## 3. Một số lưu ý khi sử dụng

- **Nhập số tương ứng** với lựa chọn bạn muốn thực hiện rồi nhấn Enter.
- Khi được yêu cầu nhập thông tin (tên đăng nhập, mật khẩu, ...), hãy làm theo hướng dẫn trên màn hình.
- Khi muốn quay lại menu chính trong quá trình nhập, có thể nhấn ESC (nếu chương trình hỗ trợ).
- Sau mỗi thao tác, chương trình thường yêu cầu bạn nhấn Enter để tiếp tục.

## 4. Kết thúc chương trình

- Chọn "3. Thoat" ở menu chính hoặc "Dang xuat" ở menu chức năng để thoát khỏi chương trình.

---

# **Tham khảo**

- cplusplus.com – Tài liệu chuẩn C++.
- cppreference.com – Tham khảo cú pháp, thư viện chuẩn.
- Stack Overflow – Tham khảo xử lý file, OTP, backup.
- Một số ý tưởng về quản lý giao dịch, rollback tham khảo từ các bài toán ngân hàng cơ bản.

---

**Mọi thắc mắc, vui lòng liên hệ son23tuoi@gmail.com để được hỗ trợ!**
