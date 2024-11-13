CC=gcc
CFLAGS=-Wall -Werror -Wextra $(LOGGING_FLAGS)
CFLAGS_TAIL=-lm -ldl

# Логирование
LOG_SRCS=$(wildcard src/common/*.c)
LOG_OBJS=$(LOG_SRCS:.c=.o)
LOGGING_FLAGS = $(shell echo $(LOG) | sed 's/\-D/\-D/g')

# Исходные файлы для библиотеки и приложения
LIB_SRCS=$(wildcard src/lib/*.c)
LIB_OBJS=$(LIB_SRCS:.c=.o)

APP_SRCS=$(wildcard src/bm_console_app/*.c)
APP_OBJS=$(APP_SRCS:.c=.o)

PLUGIN_SRCS=$(wildcard src/plugins/*.c)
PLUGIN_OBJS=$(PLUGIN_SRCS:.c=.o)

STATIC_LIB_TARGET=build/libbitmap.a
SHARED_LIB_TARGET=build/libbitmap.so

STATIC_APP_TARGET=build/console_app_static
SHARED_APP_TARGET=build/console_app_shared

LIB_TARGET=$(STATIC_LIB_TARGET)
APP_TARGET=$(STATIC_APP_TARGET)

PLUGIN_TAIL=$(STATIC_LIB_TARGET) -fPIC
LDFLAGS=$(STATIC_LIB_TARGET)

ifeq ($(LIB_TYPE), shared)
	LIB_TARGET=$(SHARED_LIB_TARGET)
	APP_TARGET=$(SHARED_APP_TARGET)
	PLUGIN_TAIL=-fPIC
	LDFLAGS=-Lbuild -lbitmap
endif

TEST_TARGET=build/check
TEST_SRCS=src/tests/*.c
TEST_FLAGS=-lcheck -lm -lsubunit -lrt -pthread

.PHONY: style clean mem test lib console_app plugins coverage

# Цель для библиотеки
lib: $(LIB_TARGET) plugins

# Цель для интерактивного режима
console_app: $(APP_TARGET)

# Цель для плагинов
plugins: build/bmap_and.so build/bmap_or.so build/bmap_xor.so build/bmap_not.so

# Цель для тестов
test: $(TEST_TARGET)

# Покрытие
coverage:
	$(CC) --coverage $(LIB_SRCS) $(PLUGIN_SRCS) $(TEST_SRCS) src/common/*.c $(TEST_FLAGS) -o greport
	./greport
	lcov -t "bitmap" -o bitmap.info -c -d .
	genhtml -o ./report bitmap.info
	rm -rf *.gcda *.gcno bitmap.info greport build/report
	mv report build

# Проверка стиля кода
style:
	clang-format -i --style=google src/lib/*.c src/lib/*.h src/tests/*.c src/tests/*.h src/bm_console_app/*.c src/bm_console_app/*.h src/plugins/*.c 

# Тестирование на утечки памяти
mem: test
	valgrind --tool=memcheck --leak-check=yes ./build/check

# Очистка
clean:
	rm -rf build/* src/lib/*.o src/bm_console_app/*.o src/plugins/*.o src/common/*.o

# Плагины
build/bmap_and.so: src/plugins/bmap_and.c $(LOG_SRCS)
	$(CC) $(CFLAGS) -shared -o build/bmap_and.so $^ $(PLUGIN_TAIL)

build/bmap_or.so: src/plugins/bmap_or.c $(LOG_SRCS)
	$(CC) $(CFLAGS) -shared -o build/bmap_or.so $^ $(PLUGIN_TAIL)

build/bmap_xor.so: src/plugins/bmap_xor.c $(LOG_SRCS)
	$(CC) $(CFLAGS) -shared -o build/bmap_xor.so $^ $(PLUGIN_TAIL)

build/bmap_not.so: src/plugins/bmap_not.c $(LOG_SRCS)
	$(CC) $(CFLAGS) -shared -o build/bmap_not.so $^ $(PLUGIN_TAIL)

# Библиотеки
$(STATIC_LIB_TARGET): $(LIB_OBJS) $(LOG_OBJS)
	mkdir -p build
	ar -rcs $@ $^

$(SHARED_LIB_TARGET):
	$(CC) $(CFLAGS) -shared -o $@ src/lib/*.c src/common/*.c -fPIC

# Интерактивный режим
$(APP_TARGET): lib $(APP_OBJS)
	$(CC) $(CFLAGS) $(APP_OBJS) $(LDFLAGS) -o $@ $(CFLAGS_TAIL)

# Тесты
$(TEST_TARGET):
	$(CC) $(CFLAGS) $(TEST_SRCS) $(PLUGIN_SRCS) $(LIB_SRCS) src/common/*.c $(TEST_FLAGS) -o $@

