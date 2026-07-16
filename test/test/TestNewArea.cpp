
//#include <iostream>
//#include <string>
//#include <limits> // 用于 std::numeric_limits
//
//using namespace std;
//
//int main() {
//    int age;
//    string firstName;
//    string fullSentence;
//
//    cout << "=== 第一步：测试 cin >> 和缓冲区陷阱 ===" << endl;
//    cout << "请输入你的年龄: ";
//    // 当你输入 "25\n" 时，cin >> 读取了 25，但 '\n' 留在了缓冲区。
//    cin >> age;
//
//    cout << "请输入你的名 (不带空格): ";
//    // 这里因为 cin >> 会自动跳过前导空白字符，所以它会跳过上一步残留的 '\n'。
//    // 读取完名字后，如果你按下回车，新的 '\n' 又留在了缓冲区。
//    cin >> firstName;
//
//    cout << "\n你输入的年龄是: " << age << ", 名字是: " << firstName << endl;
//
//    cout << "\n=== 第二步：测试 getline() 以及清理缓冲区 ===" << endl;
//    cout << "请输入一句包含空格的完整句子: ";
//
//    // 【关键修复】
//    // 在使用 getline 之前，因为前面的 cin >> 留下了一个 '\n'，
//    // 我们必须手动清除缓冲区里的残留字符，否则 getline 会直接读取那个 '\n' 导致跳过输入。
//    // cin.ignore() 的作用是丢弃缓冲区中的字符，直到遇到指定的数量或指定的分隔符。
//    cin.ignore(numeric_limits<streamsize>::max(), '\n');
//
//    // 现在缓冲区干净了，可以安全地读取一整行了
//    getline(cin, fullSentence);
//
//    cout << "你输入的句子是: [" << fullSentence << "]" << endl;
//
//    cout << "\n=== 第三步：测试 while(getline) 连续读取 ===" << endl;
//    cout << "请输入多行文本 (按 Ctrl+D(Linux/Mac) 或 Ctrl+Z 并回车(Windows) 结束): " << endl;
//
//    string line;
//    // 这里的 getline 返回 cin，cin 被隐式转换为 bool 用于判断流状态
//    while (getline(cin, line)) {
//        cout << "读取到一行: " << line << endl;
//    }
//
//    cout << "\n输入流已结束，程序退出。" << endl;
//
//    return 0;
//}


#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <stdexcept>
#include <queue>

// 模拟页表项 (PTE)
struct PageTableEntry {
    bool valid;          // 有效位
    uint32_t physicalPageNumber; // 物理页号 (有效时)
    uint32_t diskAddress;        // 磁盘地址 (无效时)
};

class MMU_Simulator {
private:
    uint32_t pageSize;
    uint32_t offsetMask;
    uint32_t offsetBits;

    // 模拟驻留在内存中的页表: Key 为 VPN, Value 为 PTE
    std::unordered_map<uint32_t, PageTableEntry> pageTable;

public:
    MMU_Simulator(uint32_t pageSizeBytes) : pageSize(pageSizeBytes) {
        // 计算偏移量所需的位数 (假设 pageSize 是 2 的幂)
        offsetBits = 0;
        uint32_t temp = pageSize;
        while (temp > 1) {
            temp >>= 1;
            offsetBits++;
        }
        // 生成用于提取偏移量的掩码 (例如 4KB -> 低 12 位全是 1)
        offsetMask = pageSize - 1;
    }

    // 暴露接口供 OS 初始化或更新页表
    void updatePageTable(uint32_t vpn, bool valid, uint32_t ppn, uint32_t diskAddr) {
        pageTable[vpn] = { valid, ppn, diskAddr };
    }

    // 核心翻译流程：模拟 CPU 给出虚拟地址，MMU 进行翻译
    uint32_t translate(uint32_t virtualAddress) {
        // 1. 拆分虚拟地址
        uint32_t offset = virtualAddress & offsetMask;      // 提取页内偏移
        uint32_t vpn = virtualAddress >> offsetBits;        // 提取虚拟页号

        std::cout << "[MMU] 收到虚拟地址: 0x" << std::hex << virtualAddress
            << " | 解析出 VPN: 0x" << vpn << " | Offset: 0x" << offset << std::endl;

        // 2. 查表
        auto it = pageTable.find(vpn);
        if (it == pageTable.end()) {
            throw std::runtime_error("[MMU Exception] Segmentation Fault: 访问了未分配的内存空间！");
        }

        // 3. 检查有效位
        if (!it->second.valid) {
            std::cout << "[MMU Exception] Page Fault! 虚拟页 0x" << vpn << " 不在主存中，需触发 OS 缺页中断！\n";
            // 真实硬件此时会触发中断向量，这里用抛出异常模拟
            throw std::runtime_error("Page Fault Triggered");
        }

        // 4. 地址拼接 (命中)
        uint32_t physicalAddress = (it->second.physicalPageNumber << offsetBits) | offset;
        std::cout << "[MMU] 命中！计算出的物理地址为: 0x" << std::hex << physicalAddress << std::endl;

        return physicalAddress;
    }
};

/* * 知识点拓展：快表 (TLB, Translation Lookaside Buffer)
 * 在真实 CPU 中，如果每次地址转换都要像上述代码一样去查询内存中的页表，速度将严重拖累 CPU。
 * 因此，MMU 内部增加了一个专门缓存 PTE 的超高速 Cache，即 TLB。
 * 绝大部分时候，VPN 可以直接在 TLB 中命中，无需访问主存页表，实现了近乎 0 周期的地址翻译。
 */


