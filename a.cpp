#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cstdio>
using namespace std;

// 全局常量
const int MAX_TOURIST = 50;
const int MAX_ROOM = 20;
const int MAX_ORDER = 100;
const char ADMIN_NAME[] = "admin";
const char ADMIN_PWD[] = "Admin1234";

// 结构体定义
struct Tourist {
    char username[20];
    char password[20];
    bool isRegistered;
};

struct Room {
    char roomId[10];
    char name[20];
    int price;
    char bookDate[20];
    int stock;
    bool isPublished;
};

struct Order {
    char orderId[10];
    char username[20];
    char roomId[10];
    char bookDate[20];
    char status[20];
    char comment[50];
    bool isValid;
};

// 全局变量
Tourist tourists[MAX_TOURIST];
Room rooms[MAX_ROOM];
Order orders[MAX_ORDER];
char currentUser[20];
int touristCount = 0;
int roomCount = 0;
int orderCount = 0;

// 函数声明 - 修复：使用const char*参数
void clearScreen();
void initData();
void saveDataToFile();
void loadDataFromFile();
bool isUsernameExist(const char username[]);
bool isUsernameValid(const char username[]);
bool isPasswordValid(const char password[]);
void touristRegister();
bool touristLogin();
bool adminLogin();
void adminMenu();
void touristMenu();
void addRoom();
void modifyRoom();
void viewOrdersAndComments();
void adminFunctionMenu();
void viewRooms();
void generateOrderId(char orderId[]);
void bookRoom();
void cancelBooking();
void viewAndDeleteOrder();
void addComment();
void touristFunctionMenu();
int strToInt(const char s[]);
int getStrLen(const char s[]);
bool strEquals(const char s1[], const char s2[]);  // 修复：使用const char*

// 主函数
int main() {
    initData();
    loadDataFromFile();
    int roleChoice;

    while (true) {
        clearScreen();
        cout << "==================== 民宿预定小程序 ====================" << endl;
        cout << "1. 经营者" << endl;
        cout << "2. 游客" << endl;
        cout << "3. 退出程序" << endl;
        cout << "========================================================" << endl;
        cout << "请选择角色（输入1-3）：";
        cin >> roleChoice;
        cin.ignore();

        switch (roleChoice) {
            case 1: 
                adminMenu(); 
                break;
            case 2: 
                touristMenu(); 
                break;
            case 3:
                saveDataToFile();
                cout << "感谢使用，再见！" << endl;
                system("pause");
                return 0;
            default:
                cout << "输入错误！请输入1-3之间的数字" << endl;
                system("pause");
        }
    }
    
    return 0;
}

// 清屏函数
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// 数据初始化
void initData() {
    touristCount = 0;
    roomCount = 0;
    orderCount = 0;
    
    for (int i = 0; i < MAX_TOURIST; i++) {
        tourists[i].isRegistered = false;
        memset(tourists[i].username, 0, sizeof(tourists[i].username));
        memset(tourists[i].password, 0, sizeof(tourists[i].password));
    }
    
    for (int i = 0; i < MAX_ROOM; i++) {
        rooms[i].isPublished = false;
        memset(rooms[i].roomId, 0, sizeof(rooms[i].roomId));
        memset(rooms[i].name, 0, sizeof(rooms[i].name));
        memset(rooms[i].bookDate, 0, sizeof(rooms[i].bookDate));
        rooms[i].price = 0;
        rooms[i].stock = 0;
    }
    
    for (int i = 0; i < MAX_ORDER; i++) {
        orders[i].isValid = false;
        memset(orders[i].orderId, 0, sizeof(orders[i].orderId));
        memset(orders[i].username, 0, sizeof(orders[i].username));
        memset(orders[i].roomId, 0, sizeof(orders[i].roomId));
        memset(orders[i].bookDate, 0, sizeof(orders[i].bookDate));
        memset(orders[i].status, 0, sizeof(orders[i].status));
        memset(orders[i].comment, 0, sizeof(orders[i].comment));
    }
    
    memset(currentUser, 0, sizeof(currentUser));
}

// 保存数据到文件
void saveDataToFile() {
    FILE* fp = fopen("user.txt", "w");
    if (fp != NULL) {
        for (int i = 0; i < touristCount; i++) {
            if (tourists[i].isRegistered) {
                fprintf(fp, "%s %s\n", tourists[i].username, tourists[i].password);
            }
        }
        fclose(fp);
    }

    fp = fopen("room.txt", "w");
    if (fp != NULL) {
        for (int i = 0; i < roomCount; i++) {
            if (rooms[i].isPublished) {
                fprintf(fp, "%s %s %d %s %d\n", rooms[i].roomId, rooms[i].name,
                        rooms[i].price, rooms[i].bookDate, rooms[i].stock);
            }
        }
        fclose(fp);
    }

    fp = fopen("order.txt", "w");
    if (fp != NULL) {
        for (int i = 0; i < orderCount; i++) {
            if (orders[i].isValid) {
                fprintf(fp, "%s %s %s %s %s %s\n", orders[i].orderId, orders[i].username,
                        orders[i].roomId, orders[i].bookDate,
                        orders[i].status, orders[i].comment);
            }
        }
        fclose(fp);
    }
}

// 加载历史数据
void loadDataFromFile() {
    FILE* fp = fopen("user.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%19s %19s", tourists[touristCount].username, 
                      tourists[touristCount].password) == 2) {
            tourists[touristCount].isRegistered = true;
            touristCount++;
            if (touristCount >= MAX_TOURIST) break;
        }
        fclose(fp);
    }

    fp = fopen("room.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%9s %19s %d %19s %d", rooms[roomCount].roomId, 
                      rooms[roomCount].name, &rooms[roomCount].price, 
                      rooms[roomCount].bookDate, &rooms[roomCount].stock) == 5) {
            rooms[roomCount].isPublished = true;
            roomCount++;
            if (roomCount >= MAX_ROOM) break;
        }
        fclose(fp);
    }

    fp = fopen("order.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%9s %19s %9s %19s %19s %49s", orders[orderCount].orderId, 
                      orders[orderCount].username, orders[orderCount].roomId, 
                      orders[orderCount].bookDate, orders[orderCount].status, 
                      orders[orderCount].comment) == 6) {
            orders[orderCount].isValid = true;
            orderCount++;
            if (orderCount >= MAX_ORDER) break;
        }
        fclose(fp);
    }
}

// 字符串长度计算 - 修复：使用const char*
int getStrLen(const char s[]) {
    int len = 0;
    while (s[len] != '\0' && len < 100) {
        len++;
    }
    return len;
}

// 字符串比较 - 修复：使用const char*
bool strEquals(const char s1[], const char s2[]) {
    int len1 = getStrLen(s1);
    int len2 = getStrLen(s2);
    if (len1 != len2) return false;
    for (int i = 0; i < len1; i++) {
        if (s1[i] != s2[i]) return false;
    }
    return true;
}

// 字符串转整数 - 修复：使用const char*
int strToInt(const char s[]) {
    int num = 0;
    int len = getStrLen(s);
    for (int i = 0; i < len; i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            num = num * 10 + (s[i] - '0');
        }
    }
    return num;
}

// 检查用户名是否存在 - 修复：使用const char*
bool isUsernameExist(const char username[]) {
    for (int i = 0; i < touristCount; i++) {
        if (tourists[i].isRegistered && strEquals(tourists[i].username, username)) {
            return true;
        }
    }
    return false;
}

// 校验用户名 - 修复：使用const char*
bool isUsernameValid(const char username[]) {
    int len = getStrLen(username);
    if (len < 1 || len > 19) return false;
    for (int i = 0; i < len; i++) {
        if (!((username[i] >= 'A' && username[i] <= 'Z') || 
              (username[i] >= 'a' && username[i] <= 'z'))) {
            return false;
        }
    }
    return true;
}

// 校验密码 - 修复：使用const char*
bool isPasswordValid(const char password[]) {
    int len = getStrLen(password);
    if (len < 8 || len > 19) return false;
    bool hasUpper = false, hasLower = false, hasNum = false;
    for (int i = 0; i < len; i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') hasUpper = true;
        else if (password[i] >= 'a' && password[i] <= 'z') hasLower = true;
        else if (password[i] >= '0' && password[i] <= '9') hasNum = true;
    }
    return hasUpper && hasLower && hasNum;
}

// 游客注册
void touristRegister() {
    clearScreen();
    char username[20], password[20], confirmPwd[20];

    cout << "==================== 游客注册 ====================" << endl;

    while (true) {
        cout << "请输入用户名（仅英文字母，1-19字符）：";
        cin >> username;

        if (!isUsernameValid(username)) {
            cout << "用户名不合法！仅含英文字母，长度1-19" << endl;
            continue;
        }

        if (isUsernameExist(username)) {
            cout << "用户名已存在！请换一个" << endl;
            continue;
        }
        break;
    }

    while (true) {
        cout << "请输入密码（大小写+数字，8-19字符）：";
        cin >> password;

        if (!isPasswordValid(password)) {
            cout << "密码不合法！需含大小写字母和数字，长度8-19" << endl;
            continue;
        }

        cout << "请再次输入密码：";
        cin >> confirmPwd;

        if (!strEquals(password, confirmPwd)) {
            cout << "两次密码不一致！" << endl;
            continue;
        }
        break;
    }

    strcpy(tourists[touristCount].username, username);
    strcpy(tourists[touristCount].password, password);
    tourists[touristCount].isRegistered = true;
    touristCount++;

    cout << "注册成功！" << endl;
    system("pause");
}

// 游客登录
bool touristLogin() {
    clearScreen();
    char username[20], password[20];
    int errorCount = 0;

    cout << "==================== 游客登录 ====================" << endl;

    while (errorCount < 3) {
        cout << "请输入用户名：";
        cin >> username;
        cout << "请输入密码：";
        cin >> password;

        bool found = false;
        for (int i = 0; i < touristCount; i++) {
            if (tourists[i].isRegistered && strEquals(tourists[i].username, username)) {
                found = true;
                if (strEquals(tourists[i].password, password)) {
                    strcpy(currentUser, username);
                    cout << "登录成功！欢迎您，" << username << "！" << endl;
                    system("pause");
                    return true;
                } else {
                    errorCount++;
                    cout << "密码错误！剩余尝试次数：" << 3 - errorCount << endl;
                    break;
                }
            }
        }

        if (!found) {
            cout << "用户名未注册！" << endl;
            system("pause");
            return false;
        }
    }

    cout << "密码错误超过3次！" << endl;
    system("pause");
    return false;
}

// 经营者登录 - 修复：const char* 类型转换问题
bool adminLogin() {
    clearScreen();
    char password[20];
    int errorCount = 0;

    cout << "==================== 经营者登录 ====================" << endl;
    cout << "用户名：" << ADMIN_NAME << endl;

    while (errorCount < 3) {
        cout << "请输入密码：";
        cin >> password;

        // 修复：创建临时char数组来比较
        char tempPwd[20];
        strcpy(tempPwd, ADMIN_PWD);
        if (strEquals(password, tempPwd)) {
            cout << "登录成功！" << endl;
            system("pause");
            return true;
        } else {
            errorCount++;
            cout << "密码错误！剩余尝试次数：" << 3 - errorCount << endl;
        }
    }

    cout << "密码错误超过3次！" << endl;
    system("pause");
    return false;
}

// 经营者功能菜单
void adminFunctionMenu() {
    int choice;
    while (true) {
        clearScreen();
        cout << "==================== 经营者功能菜单 ====================" << endl;
        cout << "1. 发布房型" << endl;
        cout << "2. 修改房型" << endl;
        cout << "3. 查看订单及评论" << endl;
        cout << "4. 返回上一级" << endl;
        cout << "========================================================" << endl;
        cout << "请选择（1-4）：";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addRoom(); break;
            case 2: modifyRoom(); break;
            case 3: viewOrdersAndComments(); break;
            case 4: return;
            default:
                cout << "输入错误！请输入1-4之间的数字" << endl;
                system("pause");
        }
    }
}

// 经营者菜单
void adminMenu() {
    int choice;
    while (true) {
        clearScreen();
        cout << "==================== 经营者菜单 ====================" << endl;
        cout << "1. 登录" << endl;
        cout << "2. 返回主菜单" << endl;
        cout << "========================================================" << endl;
        cout << "请选择（1-2）：";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                if (adminLogin()) {
                    adminFunctionMenu();
                }
                break;
            case 2:
                return;
            default:
                cout << "输入错误！请输入1-2之间的数字" << endl;
                system("pause");
        }
    }
}

// 发布房型
void addRoom() {
    clearScreen();
    char roomId[10], name[20], bookDate[20];
    int price, stock;

    cout << "==================== 发布房型 ====================" << endl;
    cout << "请输入房型ID（如R001）：";
    cin >> roomId;
    cout << "请输入房型名称：";
    cin >> name;
    cout << "请输入价格（元/晚）：";
    cin >> price;
    cout << "请输入可预订日期（格式YYYY-MM-DD）：";
    cin >> bookDate;
    cout << "请输入剩余数量：";
    cin >> stock;

    if (getStrLen(roomId) == 0 || getStrLen(name) == 0 || price <= 0 || stock <= 0) {
        cout << "输入数据不合法！" << endl;
        system("pause");
        return;
    }

    strcpy(rooms[roomCount].roomId, roomId);
    strcpy(rooms[roomCount].name, name);
    rooms[roomCount].price = price;
    strcpy(rooms[roomCount].bookDate, bookDate);
    rooms[roomCount].stock = stock;
    rooms[roomCount].isPublished = true;
    roomCount++;

    cout << "房型发布成功！" << endl;
    system("pause");
}

// 修改房型
void modifyRoom() {
    clearScreen();
    char roomId[10];
    int findIndex = -1;

    cout << "==================== 修改房型 ====================" << endl;
    cout << "请输入要修改的房型ID：";
    cin >> roomId;

    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].isPublished && strEquals(rooms[i].roomId, roomId)) {
            findIndex = i;
            break;
        }
    }

    if (findIndex == -1) {
        cout << "未找到该房型ID！" << endl;
        system("pause");
        return;
    }

    cout << "当前信息：名称=" << rooms[findIndex].name << " 价格=" << rooms[findIndex].price
         << " 日期=" << rooms[findIndex].bookDate << " 数量=" << rooms[findIndex].stock << endl;

    char newName[20], newDate[20];
    int newPrice, newStock;
    
    cout << "请输入新名称：";
    cin >> newName;
    cout << "请输入新价格：";
    cin >> newPrice;
    cout << "请输入新日期：";
    cin >> newDate;
    cout << "请输入新数量：";
    cin >> newStock;

    strcpy(rooms[findIndex].name, newName);
    rooms[findIndex].price = newPrice;
    strcpy(rooms[findIndex].bookDate, newDate);
    rooms[findIndex].stock = newStock;

    cout << "修改成功！" << endl;
    system("pause");
}

// 查看订单及评论
void viewOrdersAndComments() {
    clearScreen();
    cout << "==================== 所有订单及评论 ====================" << endl;
    
    bool hasOrders = false;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].isValid) {
            hasOrders = true;
            cout << "订单ID：" << orders[i].orderId << " 用户：" << orders[i].username
                 << " 房型：" << orders[i].roomId << " 日期：" << orders[i].bookDate
                 << " 状态：" << orders[i].status << " 评论：" 
                 << (getStrLen(orders[i].comment) > 0 ? orders[i].comment : "无") << endl;
        }
    }
    
    if (!hasOrders) {
        cout << "暂无订单！" << endl;
    }
    
    system("pause");
}

// 查看房型
void viewRooms() {
    clearScreen();
    cout << "==================== 所有房型信息 ====================" << endl;
    
    bool hasRooms = false;
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].isPublished) {
            hasRooms = true;
            cout << "房型ID：" << rooms[i].roomId << " 名称：" << rooms[i].name
                 << " 价格：" << rooms[i].price << "元/晚 日期：" << rooms[i].bookDate
                 << " 剩余：" << rooms[i].stock << "间" << endl;
        }
    }
    
    if (!hasRooms) {
        cout << "暂无房型信息！" << endl;
    }
    
    system("pause");
}

// 生成订单ID
void generateOrderId(char orderId[]) {
    srand((unsigned int)time(NULL));
    orderId[0] = 'O';
    for (int i = 1; i < 5; i++) {
        orderId[i] = rand() % 10 + '0';
    }
    orderId[5] = '\0';
}

// 预订房间
void bookRoom() {
    clearScreen();
    viewRooms();
    
    char roomId[10], bookDate[20];
    int findIndex = -1;

    cout << "==================== 预订房间 ====================" << endl;
    cout << "请输入房型ID：";
    cin >> roomId;
    cout << "请输入预订日期：";
    cin >> bookDate;

    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].isPublished && strEquals(rooms[i].roomId, roomId)) {
            findIndex = i;
            break;
        }
    }

    if (findIndex == -1) {
        cout << "房型不存在！" << endl;
        system("pause");
        return;
    }

    if (rooms[findIndex].stock <= 0) {
        cout << "该房型已售完！" << endl;
        system("pause");
        return;
    }

    rooms[findIndex].stock--;

    char orderId[10];
    generateOrderId(orderId);
    
    strcpy(orders[orderCount].orderId, orderId);
    strcpy(orders[orderCount].username, currentUser);
    strcpy(orders[orderCount].roomId, roomId);
    strcpy(orders[orderCount].bookDate, bookDate);
    strcpy(orders[orderCount].status, "已预订");
    orders[orderCount].isValid = true;
    orderCount++;

    cout << "预订成功！订单ID：" << orderId << endl;
    system("pause");
}

// 取消预订 - 修复：字符串常量问题
void cancelBooking() {
    clearScreen();
    cout << "==================== 取消预订 ====================" << endl;
    
    int cancelableCount = 0;
    int orderIndices[MAX_ORDER];
    
    // 修复：使用char数组而不是字符串常量
    char bookedStatus[20] = "已预订";
    
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].isValid && strEquals(orders[i].username, currentUser) && 
            strEquals(orders[i].status, bookedStatus)) {
            orderIndices[cancelableCount] = i;
            cout << cancelableCount + 1 << ". 订单ID：" << orders[i].orderId 
                 << " 房型：" << orders[i].roomId << " 日期：" << orders[i].bookDate << endl;
            cancelableCount++;
        }
    }

    if (cancelableCount == 0) {
        cout << "无可取消订单！" << endl;
        system("pause");
        return;
    }

    int choice;
    cout << "请选择要取消的订单序号：";
    cin >> choice;

    if (choice < 1 || choice > cancelableCount) {
        cout << "选择无效！" << endl;
        system("pause");
        return;
    }

    int orderIndex = orderIndices[choice - 1];
    strcpy(orders[orderIndex].status, "已取消");
    
    // 恢复库存
    for (int i = 0; i < roomCount; i++) {
        if (strEquals(rooms[i].roomId, orders[orderIndex].roomId)) {
            rooms[i].stock++;
            break;
        }
    }

    cout << "取消成功！" << endl;
    system("pause");
}

// 查看及删除订单
void viewAndDeleteOrder() {
    clearScreen();
    cout << "==================== 我的订单 ====================" << endl;
    
    int myOrderCount = 0;
    int orderIndices[MAX_ORDER];
    
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].isValid && strEquals(orders[i].username, currentUser)) {
            orderIndices[myOrderCount] = i;
            cout << myOrderCount + 1 << ". 订单ID：" << orders[i].orderId 
                 << " 房型：" << orders[i].roomId << " 日期：" << orders[i].bookDate
                 << " 状态：" << orders[i].status << " 评论：" 
                 << (getStrLen(orders[i].comment) > 0 ? orders[i].comment : "无") << endl;
            myOrderCount++;
        }
    }

    if (myOrderCount == 0) {
        cout << "暂无订单！" << endl;
        system("pause");
        return;
    }

    int choice;
    cout << "请输入要删除的订单序号（0返回）：";
    cin >> choice;

    if (choice == 0) return;
    if (choice < 1 || choice > myOrderCount) {
        cout << "选择无效！" << endl;
        system("pause");
        return;
    }

    orders[orderIndices[choice - 1]].isValid = false;
    cout << "删除成功！" << endl;
    system("pause");
}

// 发表评论 - 修复：字符串常量问题
void addComment() {
    clearScreen();
    cout << "==================== 发表评论 ====================" << endl;
    
    int commentableCount = 0;
    int orderIndices[MAX_ORDER];
    
    // 修复：使用char数组而不是字符串常量
    char bookedStatus[20] = "已预订";
    
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].isValid && strEquals(orders[i].username, currentUser) &&
            strEquals(orders[i].status, bookedStatus) && getStrLen(orders[i].comment) == 0) {
            orderIndices[commentableCount] = i;
            cout << commentableCount + 1 << ". 订单ID：" << orders[i].orderId 
                 << " 房型：" << orders[i].roomId << " 日期：" << orders[i].bookDate << endl;
            commentableCount++;
        }
    }

    if (commentableCount == 0) {
        cout << "无可评论订单！" << endl;
        system("pause");
        return;
    }

    int choice;
    char comment[50];
    cout << "请选择订单序号：";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > commentableCount) {
        cout << "选择无效！" << endl;
        system("pause");
        return;
    }

    cout << "请输入评论：";
    cin.getline(comment, 50);
    
    strcpy(orders[orderIndices[choice - 1]].comment, comment);
    cout << "评论成功！" << endl;
     system("pause");
}

// 游客功能菜单
void touristFunctionMenu() {
    int choice;
    while (true) {
        clearScreen();
        cout << "==================== 游客功能菜单 ====================" << endl;
        cout << "1. 查看房型" << endl;
        cout << "2. 预订房间" << endl;
        cout << "3. 取消预订" << endl;
        cout << "4. 查看及删除订单" << endl;
        cout << "5. 发表评论" << endl;
        cout << "6. 返回上一级" << endl;
        cout << "========================================================" << endl;
        cout << "请选择（1-6）：";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: viewRooms(); break;
            case 2: bookRoom(); break;
            case 3: cancelBooking(); break;
            case 4: viewAndDeleteOrder(); break;
            case 5: addComment(); break;
            case 6: return;
            default:
                cout << "输入错误！请输入1-6之间的数字" << endl;
                system("pause");
        }
    }
}

// 游客菜单
void touristMenu() {
    int choice;
    while (true) {
        clearScreen();
        cout << "==================== 游客菜单 ====================" << endl;
        cout << "1. 注册" << endl;
        cout << "2. 登录" << endl;
        cout << "3. 返回主菜单" << endl;
        cout << "========================================================" << endl;
        cout << "请选择（1-3）：";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: touristRegister(); break;
            case 2:
                if (touristLogin()) {
                    touristFunctionMenu();
                }
                break;
            case 3: return;
            default:
                cout << "输入错误！请输入1-3之间的数字" << endl;
                system("pause");
        }
    }
}



