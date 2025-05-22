# Tên trình biên dịch C (sử dụng gcc của MINGW64)
CC = gcc

# Cờ biên dịch (thêm -Wall để bật cảnh báo, -g để debug nếu cần)
CFLAGS = -Wall -g -v

# Bỏ biến INCLUDE_PATH vì GCC đã tự động tìm kiếm đường dẫn này

# Cờ liên kết và các thư viện cần liên kết
# Vì bạn đã cài đặt GLEW và GLFW bằng Pacman trong MINGW64,
# trình biên dịch thường có thể tìm thấy header và lib mà không cần -I và -L
# Các thư viện hệ thống Windows cần thiết cho OpenGL/GLFW
LIBS = -lglfw3 -lglew32 -lopengl32 -lgdi32 -luser32 -lkernel32

# Tên file thực thi (trên Windows là .exe)
TARGET = buntesdreieck.exe

# File nguồn C
SRC = buntesdreieck.c

# File đối tượng (kết quả sau khi biên dịch file nguồn)
# Sử dụng thay thế hậu tố để chuyển .c thành .o
OBJ = $(SRC:.c=.o)

# Target mặc định: xây dựng file thực thi
# Khi chạy 'make' mà không có đối số, target 'all' sẽ được thực thi
all: $(TARGET)

# Quy tắc để liên kết các file đối tượng thành file thực thi
# Phụ thuộc vào file đối tượng $(OBJ)
# $^: tất cả các prerequisite (ở đây là buntesdreieck.o)
# $@: target (ở đây là buntesdreieck.exe)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS)

# Quy tắc để biên dịch file nguồn C (.c) thành file đối tượng (.o)
# Đây là quy tắc chung (pattern rule) áp dụng cho bất kỳ file .c nào
# Phụ thuộc vào file nguồn $< (file .c)
# $@: target (file .o)
# Không cần sử dụng $(INCLUDE_PATH) ở đây nữa
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target 'clean' để xóa các file đã tạo ra trong quá trình build
# Không phụ thuộc vào file nào cả
clean:
	# Xóa file thực thi và các file đối tượng
	rm -f $(TARGET) $(OBJ)