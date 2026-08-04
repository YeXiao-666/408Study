# 









# OS强化

![1ba6d442-7a9b-419d-9263-cd9343f42425](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/1ba6d442-7a9b-419d-9263-cd9343f42425.png) ![9c4d0fc6-be0c-4587-9c69-318791b1c6ee](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/9c4d0fc6-be0c-4587-9c69-318791b1c6ee.png)



![64493da7-0933-4891-9787-8873fda33c16](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/64493da7-0933-4891-9787-8873fda33c16.png)![006fa141-d27e-4239-be99-fe72b6b3738a](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/006fa141-d27e-4239-be99-fe72b6b3738a.png)







![48d51abe-7624-4c02-99fb-03ac89baeb53](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/48d51abe-7624-4c02-99fb-03ac89baeb53.png)

除了IO，磁盘相关的考点有深入的变化，其他方向的重点依旧不变，pv，文件，内存管理的分页管理

![481dfb11-1823-458f-bf2d-eacb853fcdc1](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/481dfb11-1823-458f-bf2d-eacb853fcdc1.png)![ab00a047-0c30-4563-ac1c-7728501f2186](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/ab00a047-0c30-4563-ac1c-7728501f2186.png)



# 一、进程管理 PV 操作



![05418dad-c159-48f8-94b6-e9a78df4de54](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/05418dad-c159-48f8-94b6-e9a78df4de54.png)![5f85eb6f-6591-42f7-a2c0-7bb703195703](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/5f85eb6f-6591-42f7-a2c0-7bb703195703.png)



## 生产者消费者问题

### 解题顺序：



<u>通常互斥信号量mutex 应在 同步信号量 pv之内，不会发生死锁</u>

![865b6d65-c665-44fb-9aa1-e1cd9b1d7c2f](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/865b6d65-c665-44fb-9aa1-e1cd9b1d7c2f.png)

![fa2852b3-7dd1-4efa-a0c3-2815f5023a37](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/fa2852b3-7dd1-4efa-a0c3-2815f5023a37.png)







#### 题型一：出入口，只需做一次的动作，无需循环









![1cbab1f0-955e-4ee6-b23c-05877176f6eb](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/1cbab1f0-955e-4ee6-b23c-05877176f6eb.png)![d2f461d0-6fcd-41a0-a3f2-8ab59173ca3b](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/d2f461d0-6fcd-41a0-a3f2-8ab59173ca3b.png)![f90dc3f6-8264-4569-9000-9f09d16921a2](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/f90dc3f6-8264-4569-9000-9f09d16921a2.png)







#### 题型二：动作需要一直循环



![8966f705-e689-49eb-9d33-0c80b40606d3](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/8966f705-e689-49eb-9d33-0c80b40606d3.png)![8ee75c50-20d9-47dc-a057-ec04fedb2a69](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/8ee75c50-20d9-47dc-a057-ec04fedb2a69.png)![f100beab-f982-4919-94bd-f3249f38e052](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/f100beab-f982-4919-94bd-f3249f38e052.png)

![985bbe48-04ac-4490-a9e7-06549b7743a3](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/985bbe48-04ac-4490-a9e7-06549b7743a3.png)



#### 题型三：给出第一二步



![95f6b1ed-3a69-42e8-9d87-5bf03ae9f2d5](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/95f6b1ed-3a69-42e8-9d87-5bf03ae9f2d5.png)



![f84ae61e-353b-434c-ba12-87c2cf925efd](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/f84ae61e-353b-434c-ba12-87c2cf925efd.png)









#### 题型四：单纯的同步问题，较简单

![58627be8-39f0-4326-889c-baf724bff174](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/58627be8-39f0-4326-889c-baf724bff174.png)

![6844de1d-94ad-473b-b861-786eedd1cbe1](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/6844de1d-94ad-473b-b861-786eedd1cbe1.png)![04c92424-db80-41df-b350-ff437b08c061](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/04c92424-db80-41df-b350-ff437b08c061.png)







+Swap![dfd19d48-bad4-4f38-8f77-77ae7bbfe01d](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/dfd19d48-bad4-4f38-8f77-77ae7bbfe01d.png)![6df0f2a5-36be-498d-adf4-08c9a3118bd6](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/6df0f2a5-36be-498d-adf4-08c9a3118bd6.png)![fd4f9876-1b6a-4a30-a1bb-a3d1844aa0cb](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/fd4f9876-1b6a-4a30-a1bb-a3d1844aa0cb.png)![7fecc870-4d10-46ed-91ce-1fbf14ed664b](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/7fecc870-4d10-46ed-91ce-1fbf14ed664b.png)





### 读者写者问题



![f2ddd3ef-2415-458c-b28e-28b54b6c5990](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/f2ddd3ef-2415-458c-b28e-28b54b6c5990.png)





### 哲学家问题



![e291bd01-db5c-43ec-8d27-493c741a54cf](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/e291bd01-db5c-43ec-8d27-493c741a54cf.png)

![3b896080-5a06-4cef-9602-ea18b2ae712e](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/3b896080-5a06-4cef-9602-ea18b2ae712e.png)



#### ！！！！解题方法3：通用：！！！！

![76089cc5-96d9-4531-a9a4-86c230fc7202](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/76089cc5-96d9-4531-a9a4-86c230fc7202.png)

![655dfc9a-0fd3-4fb5-a417-f1fd46643315](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/655dfc9a-0fd3-4fb5-a417-f1fd46643315.png)![57ff397a-a9ee-4f5d-94fb-e01212a7fbd2](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/57ff397a-a9ee-4f5d-94fb-e01212a7fbd2.png)![5dffb8d0-aadd-4e72-82aa-b33ac13f4cf4](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/5dffb8d0-aadd-4e72-82aa-b33ac13f4cf4.png)





### 理发师问题



![01e0f996-f888-4009-a329-245547d8094b](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/01e0f996-f888-4009-a329-245547d8094b.png)





# 存储系统

## !!!!页式存储!!!!! 页表的深入和TLB的大致原理

![62bbb4d0-e921-4271-8510-c22c1c212087](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/62bbb4d0-e921-4271-8510-c22c1c212087.png)

![7f2babd7-c9ed-4bb7-9150-1ff2cdf4878e](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/7f2babd7-c9ed-4bb7-9150-1ff2cdf4878e.png)



所有进程的虚拟地址VA（独属于自己的一份虚拟空间）从0开始，<u>并且在共享同一份内核区</u>（由操作系统管理, 在物理上的同一份内核区）

运行程序时，操作系统将进程装入内存的同时，（建立进程的页表）将进程的虚拟空间与物理空间的映射记录到内存的页表（本质为数组，数组元素即页表项为一个struct结构体）中（内核区），进程的PCB存放<u>页表在内存中的起始物理地址</u>。（当进程被调度时，OS将进程PCB中的页表始址写入页表始址寄存器）

![7f81bc59-5c51-4a84-94ea-87722d8e9102](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/7f81bc59-5c51-4a84-94ea-87722d8e9102.png)



![c775d6d6-3c88-41cf-9dfb-08776daf0d25](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/c775d6d6-3c88-41cf-9dfb-08776daf0d25.png)![40f516d0-f9d7-4543-bfac-446c575dcd9d](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/40f516d0-f9d7-4543-bfac-446c575dcd9d.png)



第一个页目录号的页框号为下一级页表（每个页目录对应一个页表, 即内存中有多个页表）的物理起始地址

![7b33e2c3-a2ed-4bb9-989f-0e27ed745032](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/7b33e2c3-a2ed-4bb9-989f-0e27ed745032.png) 



一级页表常驻内存，二级页表可能不在

![586ca357-d8b1-4ed3-900b-2554f8fb3eb9](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/586ca357-d8b1-4ed3-900b-2554f8fb3eb9.png)











### 真题

![e28ba9d0-2f1f-4a34-8866-bcd1b7a1928c](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/e28ba9d0-2f1f-4a34-8866-bcd1b7a1928c.png)

![8eeae2f2-5e85-4aca-beb7-bd0f851e9776](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/8eeae2f2-5e85-4aca-beb7-bd0f851e9776.png)





![297b43a6-9973-4425-b574-bb80ff24b453](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/297b43a6-9973-4425-b574-bb80ff24b453.png)![9047f68b-2af8-47bc-a9e2-e3d9a9ede46b](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/9047f68b-2af8-47bc-a9e2-e3d9a9ede46b.png)

![702c0670-742d-4fd0-8ade-b774362ff77f](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/702c0670-742d-4fd0-8ade-b774362ff77f.png)![896b874c-8dee-4c47-892c-95c55793611f](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/896b874c-8dee-4c47-892c-95c55793611f.png)![46e0baf8-7284-47d0-88f6-3831e79802cb](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/46e0baf8-7284-47d0-88f6-3831e79802cb.png)







# 文件系统

![e9a29551-2ace-4985-9834-7fbf4fe6275e](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/e9a29551-2ace-4985-9834-7fbf4fe6275e.png)![c921bc3f-9343-4d84-b2a8-3ad06eaf78a2](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/c921bc3f-9343-4d84-b2a8-3ad06eaf78a2.png)

5次IO磁盘， 通过创建进程打开文件表项，指向系统打开文件表对应文件inode编号的表项，若没有则将该inode结点从磁盘读入内核缓冲区，在系统打开文件表中创建新项，打开计数初始化为1，将进程打开文件表的对应项指向该项。最终返回给用户文件描述符（该文件在系统文件表的的索引号）![c86eb0ce-0da8-4d19-a052-87d046eab890](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/c86eb0ce-0da8-4d19-a052-87d046eab890.png)![a4d7d1cb-3327-4ce4-addb-bb93ec25f760](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/a4d7d1cb-3327-4ce4-addb-bb93ec25f760.png)

read读磁盘的操作次数 ![773638a0-af31-4052-9aef-993eebeb1737](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/773638a0-af31-4052-9aef-993eebeb1737.png)![f0711c3e-c356-4154-99f9-69973d02b72b](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/f0711c3e-c356-4154-99f9-69973d02b72b.png)![58124105-1128-4d29-a3b1-32ccfcbf3fe6](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/58124105-1128-4d29-a3b1-32ccfcbf3fe6.png)





# IO大题

![cb6be1d6-75e3-42bc-987d-b0f4df4ab006](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/cb6be1d6-75e3-42bc-987d-b0f4df4ab006.png)![2a82d928-c9b5-428e-b6e6-7f11b98029a1](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/2a82d928-c9b5-428e-b6e6-7f11b98029a1.png)![28f23eb0-3fc8-4a57-94c5-e0725e93a219](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/28f23eb0-3fc8-4a57-94c5-e0725e93a219.png)![798e3c71-5e97-4a42-ad88-5d7d53447e67](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/798e3c71-5e97-4a42-ad88-5d7d53447e67.png)![14f50cea-2397-4cb1-b831-f968926fe796](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/14f50cea-2397-4cb1-b831-f968926fe796.png)![9a75e56c-28fa-4e76-8c68-ccfef9bb7f36](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/9a75e56c-28fa-4e76-8c68-ccfef9bb7f36.png)

IO模型![2d1c28d7-1de9-43af-9145-47bd75c31d0a](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/2d1c28d7-1de9-43af-9145-47bd75c31d0a.png)![d5849bcb-6b26-4474-9c22-e509129685d7](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/d5849bcb-6b26-4474-9c22-e509129685d7.png)![9e6ebd20-1b1c-44f5-a0d0-aeba47ae1d06](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/9e6ebd20-1b1c-44f5-a0d0-aeba47ae1d06.png)![73de562e-5024-4aa5-916c-eeccaa3364cf](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/73de562e-5024-4aa5-916c-eeccaa3364cf.png)![3cd983b8-0f16-4e5d-a548-23788be47c5a](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/3cd983b8-0f16-4e5d-a548-23788be47c5a.png)![3b6a698c-7936-49d1-93ef-e468195c83d7](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/3b6a698c-7936-49d1-93ef-e468195c83d7.png) ![af160173-92f3-4275-a8a3-378234761021](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/af160173-92f3-4275-a8a3-378234761021.png)![3c6137ad-ba86-43ca-b567-16de2d2aa556](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/3c6137ad-ba86-43ca-b567-16de2d2aa556.png)![de19faff-b377-4980-aa3f-f50bc1430968](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/de19faff-b377-4980-aa3f-f50bc1430968.png)![b4e3ffcc-d5ef-490e-ae23-699bc3366d87](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/b4e3ffcc-d5ef-490e-ae23-699bc3366d87.png)![dbaad35a-e155-42e2-aa78-b8c23fd6c985](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/dbaad35a-e155-42e2-aa78-b8c23fd6c985.png)

#### DMA



![a5b1c5b9-8d52-4ec5-8d87-c8cbe2a52875](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/a5b1c5b9-8d52-4ec5-8d87-c8cbe2a52875.png)
