CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra # استفاده از استاندارد C++20 [cite: 267]
TARGET = CarWashManager

# لیست فایل‌های ابجکت
OBJS = main.o CarWashManager.o Stage.o Worker.o Car.o

# دستور اصلی ساخت برنامه
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# قوانین کامپایل هر فایل
main.o: main.cpp CarWashManager.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

CarWashManager.o: CarWashManager.cpp CarWashManager.hpp Stage.hpp Worker.hpp Car.hpp
	$(CXX) $(CXXFLAGS) -c CarWashManager.cpp

Stage.o: Stage.cpp Stage.hpp Worker.hpp Car.hpp
	$(CXX) $(CXXFLAGS) -c Stage.cpp

Worker.o: Worker.cpp Worker.hpp Car.hpp
	$(CXX) $(CXXFLAGS) -c Worker.cpp

Car.o: Car.cpp Car.hpp Types.hpp
	$(CXX) $(CXXFLAGS) -c Car.cpp

# پاک کردن فایل‌های ساخته شده
clean:
	rm -f $(OBJS) $(TARGET)