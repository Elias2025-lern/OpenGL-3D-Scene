CC = gcc

# Cờ biên dịch
# -Wall: Bật tất cả cảnh báo
# -g: Thêm thông tin debug
# -v: Hiển thị chi tiết quá trình biên dịch (có thể bỏ đi nếu không cần)
# -DGLEW_STATIC: RẤT QUAN TRỌNG cho việc liên kết GLEW tĩnh (glew32s.lib)
# -Iinclude: Thêm thư mục 'include' vào đường dẫn tìm kiếm header files
CFLAGS = -Wall -g -v -DGLEW_STATIC -Iinclude

# Thư viện cần liên kết (đã bao gồm các thư viện bạn cung cấp)
# -lglfw3: Thư viện GLFW
# -lglew32: Thư viện GLEW (thường là glew32s.lib cho static link)
# -lopengl32: Thư viện OpenGL trên Windows
# -lgdi32 -luser32 -lkernel32: Các thư viện hệ thống Windows cần thiết cho OpenGL/GLFW
LIBS = -lglfw3 -lglew32 -lopengl32 -lgdi32 -luser32 -lkernel32

# Tên file thực thi của dự án chúng ta
TARGET = comgraph3d_app.exe

# Các file nguồn C của dự án
SRC_DIR = src
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/utils.c $(SRC_DIR)/shader.c

# Thư mục chứa các file đối tượng (.o)
OBJ_DIR = obj
# Chuyển đổi các file .c trong SRCS thành các file .o trong OBJ_DIR
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Target mặc định: xây dựng file thực thi
all: $(OBJ_DIR) $(TARGET)

# Quy tắc để tạo thư mục OBJ_DIR nếu chưa tồn tại
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Quy tắc để liên kết các file đối tượng thành file thực thi
# Phụ thuộc vào tất cả các file đối tượng trong $(OBJ)
# $(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS): Lệnh biên dịch và liên kết
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS)

# Quy tắc để biên dịch file nguồn C (.c) thành file đối tượng (.o)
# Đây là quy tắc mẫu (pattern rule) áp dụng cho bất kỳ file .c nào
# $<: Tên file nguồn (ví dụ: src/main.c)
# $@: Tên file đối tượng đích (ví dụ: obj/main.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target 'clean' để xóa các file đã tạo ra trong quá trình build
clean:
	rm -f $(TARGET) $(OBJ)
	rmdir $(OBJ_DIR) 2> /dev/null || true # Xóa thư mục obj, bỏ qua lỗi nếu thư mục không rỗng