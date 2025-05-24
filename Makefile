# Tên mục tiêu (executable)
TARGET = app

# Thư mục nguồn và header
SRCDIR = src
INCDIR = include

# Thư mục chứa các tệp đối tượng trung gian
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj

# Compiler và cờ biên dịch
CC = gcc
CFLAGS = -Wall -std=c11 -I$(INCDIR) -I/mingw64/include

# Cờ linker và thư viện
# Thư viện cho Windows (MinGW): GLEW, GLFW, OpenGL32, GDI32
LDFLAGS = -lglew32 -lglfw3 -lopengl32 -lgdi32

# Tìm tất cả các tệp .c trong thư mục nguồn
SRC = $(wildcard $(SRCDIR)/*.c)

# Chuyển đổi danh sách tệp .c thành danh sách tệp .o
# Ví dụ: src/main.c -> build/obj/main.o
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

# Mục tiêu mặc định: xây dựng ứng dụng
all: $(TARGET)

# Tạo thư mục build và obj nếu chúng chưa tồn tại
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(OBJDIR): | $(BUILDDIR) # Đảm bảo BUILDDIR được tạo trước OBJDIR
	mkdir -p $(OBJDIR)

# Quy tắc chính để liên kết các tệp đối tượng thành tệp thực thi
$(TARGET): $(OBJDIR) $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

# Quy tắc để biên dịch từng tệp .c thành tệp .o
# Các tệp .o sẽ được đặt trong thư mục $(OBJDIR)
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/obj_loader.h # Thêm obj_loader.h làm dependency
	$(CC) $(CFLAGS) -c $< -o $@

# Mục tiêu dọn dẹp
clean:
	@echo "Cleaning up..."
	@del /Q $(TARGET) 2>nul || true # Xóa TARGET, không báo lỗi nếu không tìm thấy
	@rmdir /S /Q $(OBJDIR) 2>nul || true # Xóa thư mục OBJDIR và nội dung, không báo lỗi
	@rmdir /S /Q $(BUILDDIR) 2>nul || true # Xóa thư mục BUILDDIR, không báo lỗi
	@echo "Clean complete."

.PHONY: all clean