# 









# OS强化

![1ba6d442-7a9b-419d-9263-cd9343f42425](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/1ba6d442-7a9b-419d-9263-cd9343f42425.png) ![9c4d0fc6-be0c-4587-9c69-318791b1c6ee](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/9c4d0fc6-be0c-4587-9c69-318791b1c6ee.png)



![64493da7-0933-4891-9787-8873fda33c16](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/64493da7-0933-4891-9787-8873fda33c16.png)![006fa141-d27e-4239-be99-fe72b6b3738a](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/006fa141-d27e-4239-be99-fe72b6b3738a.png)







# 大题





![48d51abe-7624-4c02-99fb-03ac89baeb53](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/48d51abe-7624-4c02-99fb-03ac89baeb53.png)

除了IO，磁盘相关的考点有深入的变化，其他方向的重点依旧不变，pv，文件，内存管理的分页管理

![481dfb11-1823-458f-bf2d-eacb853fcdc1](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/481dfb11-1823-458f-bf2d-eacb853fcdc1.png)![ab00a047-0c30-4563-ac1c-7728501f2186](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/ab00a047-0c30-4563-ac1c-7728501f2186.png)



## 一、进程管理 PV 操作



![05418dad-c159-48f8-94b6-e9a78df4de54](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/05418dad-c159-48f8-94b6-e9a78df4de54.png)![5f85eb6f-6591-42f7-a2c0-7bb703195703](file:///C:/Users/DELL/Desktop/%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/408Notes/Typedown/5f85eb6f-6591-42f7-a2c0-7bb703195703.png)



### 生产者消费者问题

#### 解题顺序：



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








