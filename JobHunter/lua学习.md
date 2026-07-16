# Lua学习日志

一、lua元方法

![578062c7-5db7-41f3-baac-3bab4d016a2b](file:///C:/Users/DELL/Pictures/Typedown/578062c7-5db7-41f3-baac-3bab4d016a2b.png)

### **二、、pairs与ipairs****区别**

两者都通过迭代器实现

`ipairs`的迭代器，它遍历数组部分，按索引顺序遍历，直到遇到nil，只遍历键为数字的部分，并且索引必须连续。

`pairs`在遍历数组部分时也是按顺序遍历连续整数索引，然后遍历哈希部分（无序）。

Lua中的`pairs`实际上是通过`next`函数实现的，`next`函数是遍历整个表（包括数组和哈希）的内置函数，它返回任意顺序的键值对



### **六、如何实现一个只读表**

（1）实现只读函数：

local function readOnly(t)
    local newT = {};
    local mt = {
        __index = t,
        __newindex = function()
            error("别修改我！我是只读的！");
        end
    }
    setmetatable(newT, mt);
    return newT;
end
local days = readOnly({"星期一", "星期二", "星期日"});

days[2] = "星期三哪去了啊？" ;

【解】此法女少在，调用该方法时返回了一个空表，然后通过__newindex方法禁止赋值。



（2）当我们只想单纯的调用table里的字段或者给table字段赋值时，我们可以通过rawget函数来忽略元表的__index作用，只从table中  
查找例如：
    local father = {
        --house=1,
        sayHello = function()
            print("大家好，我是father.");
        end
    }
    local temp ={
        __index = father,
        __newindex = function(table, key)
            print(key .. "字段是不存在的，不允许给它赋值！");
        end
    }
    son = {
        car=1
    }
    setmetatable(son, temp) --把son的metatable设置为father
    print(rawget(son, "house"));
    print(rawget(son, "car"));
    rawset(son, "house", "10");
    rawset(son, "sayHello", function()
            print('============')
        end);
    print(son.house);
    son.sayHello()

### **五、_rawset与_rawget的区别**

        _rawget:访问时，不想从__index 对应的元方法中查询值

        _rawset:更新时，不想执行__newindex 对应的元方法





## 三、.Lua如何实现面向对象

 继承：       

 Lua面向对象编程是基于元表metatable，元方法__index来实现的。         通过元表的__index元方法，将一个table的__index元方法设置为另一个table，那么后者的方法就被前者继承。          如果访问了lua表中不存在的元素时，就会触发lua的一套查找机制，也是凭借这个机制，才能够实现面向对象的。          总结元表的查找步骤：          在表中查找该元素，如果找到，返回该元素，找不到则判断该表是否有元表（setmetatable(A,B)     -- 把 B设为 A的元表 ），如果没有元表，返回nil，有元表则判断元表有没有__index方法（不是直接看元表有该元素！），如果__index方法为nil，则返回nil；如果__index方法是一个表，则重复上述步骤；如果__index方法是一个函数，则调用该函数，并返回该函数的返回值。

封装：
        1.lua的表可以同时存储数据和函数，可以实现简单的封装。
        2.闭包隐藏内部状态，只暴露了必要接口，也实现了封装。
        3.通过元表实现类的封装，可以模拟面向对象编程，支持继承。
        4.将相关函数和数据封装在一个模块中，通过模块接口对外提供服务，便于复用和管理。

多态：
        多态本质也是通过元表和表的机制来模拟多态的行为。
        1.动态绑定：将利用元表的__index方法指定一个表后，有了继承关系，重写了父类里面的函数。
        2.方法重载：根据传参类型不同，执行不同的逻辑。



# 四、__index 与 __newindex的区别

注意：__index 和__newindex归为元表方法。

  __index：
        当子脚本不存在被访问的元素的时候，lua就会调用__index，去父脚本里面查找该元素。
            参数：
                如果__index是一个函数，那么参数是：table：被访问的表；key：被访问的键。
                如果__index是一个表，Lua会在这个表里面查找key对应的值。

  __newindex：
        当赋值时，如果赋值一个不存在的索引。
        如果__newindex是一个表，那么会把这个值赋值到newindex所指的表中（有这个索引就修改，没有就创建），不会修改自己；
        如果__newindex是一个函数，那么会调用这个函数。
            参数：        
       table：被赋值的表。
       key：被赋值的键。
       value：被赋值的值。



# **七、Table**

为了提高table的插入查找效率，在table的设计上，采用了 array数组 和 hashtable(哈希表) 两种数据的结合。        

数组部分：从 1 开始做整数数字索引，这可以提供紧凑且高效的随机访问；数组部分存储在 TValue *array 中，其长度信息存储在 int sizearray 中。          

哈希表：存储在 Node *node，哈希表的大小用 lu_byte lsizenode 表示，lsizenode表示的是2的几次幂，而不是实际大小，因为哈希表的大小一定是 2 的整数次幂。哈希冲突后，采取开放定址法，应对 hash 碰撞。          

每个 Table 结构，最多会由三块连续内存构成：                

 (1) 一个 table 结构                 (2) 一块存放了连续整数索引的数组                 (3) 一块大小为 2 的整数次幂的哈希表

 所以table会将部分整形key作为下标放在数组中, 其余的整形key和其他类型的key都放在hash表中。

  table中的hash表的实现结合了以上两种方法的一些特性：
                查找和插入等同链地址法（拉链法）复杂度。 O(1)
                内存开销近似等同于开放定址法。O(n)

八、string 类型
        string：底层实现对字符串做了分类处理。
        只要大于 40 字节的字符串，即使内容一样，也会在堆内存中，重新生成一个全新的字符串数据拷贝；
        当小于 40 字节时，使用全局 stringtable 进行管理，相同字符串只会有一份数据拷贝，每份相同的对象只是存在一个 hash 值，用来索引 stringtable。





# XLua框架与热更新

![2f8e344f-c0f0-4caf-b256-edf1adf2b712](file:///C:/Users/DELL/Pictures/Typedown/2f8e344f-c0f0-4caf-b256-edf1adf2b712.png)

## 前提知识了解（路径）

| **路径名称 (C#封装)**           | **读写权限 (事实属性)** | **核心应用场景**                 | **多平台底层映射差异**                                            |
| ------------------------- | --------------- | -------------------------- | -------------------------------------------------------- |
| **`dataPath`**            | 绝大平台**只读**      | 编辑器模式下读取项目资源、加载包体核心数据。     | PC指向`XXX_Data`，安卓指向`base.apk`内部，通常发布后**不可写且受限**。         |
| **`persistentDataPath`**  | **可读写**         | 玩家存档、用户配置、热更新下载的数据文件保存。    | 跨平台最稳定，映射到系统分配给应用的**沙盒私有目录**，卸载前不丢失。                     |
| **`streamingAssetsPath`** | 移动平台**严格只读**    | 随包分发的初始配置文件、大体积音视频（无需修改）。  | 随包体原样打包。PC上直接路径访问，安卓/iOS需通过特殊请求（如`UnityWebRequest`）流式读取。 |
| **`temporaryCachePath`**  | **可读写** (临时性)   | 网络下载的图片/视频缓存、运行时产生的中间临时数据。 | 映射到系统缓存目录。**状态不稳定**，系统可能会自动清理。                           |

## 资源加载流程

版本文件是热更新中的从资源服务器下载过来的文件列表。

![be0ecfeb-5bfb-44f0-ab33-2668989c9251](file:///C:/Users/DELL/Pictures/Typedown/be0ecfeb-5bfb-44f0-ab33-2668989c9251.png)

##### <1> AssetBundle包的Build

###### AssetBundleBuild类   需要获取assetNames 和 assetBundleName （bundleName + ' .ab ' (后缀)）

##### 解析版本文件

按文件打包: 

**获取文件信息**（1）读取Assets/BuildResources目录下的所有需要打包的文件files

（2）去掉files中所有的meta文件

（3）将文件路径中的 ' \ ' 转换为 ‘ / ’  ， 即fileName ：.../ + assetName （ Assets/BuildResources/....）

（4）bundleName 将 .../Assets/BuildResources/全部去掉,  只保留在BuildResources/中文件的路径。

（5）assetBundleName = bundleName + ' .ab '

（6）将该AssetBundleBuild 添加到打包列表中。

（7）遍历files中的每个文件重复上述操作

（8）建立（删除已有BundleOut路径文件夹），使用BuildPipeline.BuildAssetBundles对打包文件列表进行打包。

（9）创建版本文件  fileList， AssetDatabase.Refresh();//更新FileList显示，在StreamingAssets目录下，刷新出已打出的版本文件

```
if (Directory.Exists(PathUtil.BundleOutPath))
    Directory.Delete(PathUtil.BundleOutPath, true); // true 递归删除

Directory.CreateDirectory(PathUtil.BundleOutPath); // 建立新的bundle文件夹
BuildPipeline.BuildAssetBundles(PathUtil.BundleOutPath, assetBundleBuilds.ToArray(), BuildAssetBundleOptions.None, BuildTarget.StandaloneWindows);

File.WriteAllLines(PathUtil.BundleOutPath + "/" + AppConst.FileListName, bundleInfos); //创建文件信息文件

AssetDatabase.Refresh();//更新FileList显示

```

加载文件数据：

加载完文件的ab包后，需要加载其所有依赖资源dependencesList，才能将其完整的加载出来。



##### <2>资源加载

（1）

（1）**<u>解析StreamAssets（或者persistentData）中的版本文件</u>**：将fileList中的数据每一行读取出来，每一行都是一个BundleInfo，第一个|前是AssetName 后是BundleName 第二个|后是Dependences依赖信息。 将所有BundleInfo作为Value，assetName作为Key保存在 存放bundle信息的字典中。

（2）**<u>异步加载资源（Bundle）</u>**：递归加载依赖资源； AssetBundleCreateRequest创建Bundle请求， AssetBundleRequest加载Bundle请求，`action?.Invoke(bundleRequest?.asset);`调用回调函数action（利用asset创建实例对象）。

##### <3>资源卸载（ 待完成）

##### <4>资源路径规划

![a8442826-5ccb-4435-8d8a-dc92aa6ac500](file:///C:/Users/DELL/Pictures/Typedown/a8442826-5ccb-4435-8d8a-dc92aa6ac500.png)

##### <5>编辑器模式下加载（ 即程序员打bundle避免频繁）



## 热更新流程

### 方案：

![a0909655-6e96-41ef-bf2b-10bdb4e7f6cd](file:///C:/Users/DELL/Pictures/Typedown/a0909655-6e96-41ef-bf2b-10bdb4e7f6cd.png)





### 资源更新

![4c82fe88-f741-48ca-b1a4-29af88b22f46](file:///C:/Users/DELL/Pictures/Typedown/4c82fe88-f741-48ca-b1a4-29af88b22f46.png) 

![03a6ea40-c43a-435c-871a-d9b7eb899282](file:///C:/Users/DELL/Pictures/Typedown/03a6ea40-c43a-435c-871a-d9b7eb899282.png)

![ae5c7ea8-0b94-4de0-a0d5-ba851dd173ca](file:///C:/Users/DELL/Pictures/Typedown/ae5c7ea8-0b94-4de0-a0d5-ba851dd173ca.png)









文件路径：

![571b3532-2b18-42d5-b01b-e74bab026bef](file:///C:/Users/DELL/Pictures/Typedown/571b3532-2b18-42d5-b01b-e74bab026bef.png)



## Lua管理

![316ac4ef-13f2-462a-904a-a8f9ce452957](file:///C:/Users/DELL/Pictures/Typedown/316ac4ef-13f2-462a-904a-a8f9ce452957.png)





## LuaC#调用

具体流程：



![663d0980-9c42-47f1-a32f-99896f65e57b](file:///C:/Users/DELL/Pictures/Typedown/663d0980-9c42-47f1-a32f-99896f65e57b.png)

（1）在LuaBehaviour中定义C#需要调用Lua的方法 ray，

（2）接收lua中对应名称的方法，使用scriptEnv.Get（“ray”，out ray），从环境中取出lua脚本中方法ray的脚本环境，交给C#中的 ray方法。

（3）在C#中使用该方法时，将该方法所需传递的参数传给LuaBehaviour.ray(......)

（4）Lua中main.lua 定义ray方法，使用Event.call（。。。。。）传递参数，传给具体的其他监听ray AddListener的lua脚本，

（5）监听ray事件的脚本接收参数，执行相应方法

![a2ea11e2-602c-4409-ac11-103d3b4153a6](file:///C:/Users/DELL/Pictures/Typedown/a2ea11e2-602c-4409-ac11-103d3b4153a6.png)

![f1ef7c0b-67a0-4006-baa6-326bb131b2d9](file:///C:/Users/DELL/Pictures/Typedown/f1ef7c0b-67a0-4006-baa6-326bb131b2d9.png)

![9a2feb97-f2e0-4c8c-b2e3-aacc7f4d0767](file:///C:/Users/DELL/Pictures/Typedown/9a2feb97-f2e0-4c8c-b2e3-aacc7f4d0767.png)

![ecb3733b-b5ea-4ce7-b59e-60420769367c](file:///C:/Users/DELL/Pictures/Typedown/ecb3733b-b5ea-4ce7-b59e-60420769367c.png)

![59fe7faf-081c-4d3b-8e2d-9e8b844dc1b0](file:///C:/Users/DELL/Pictures/Typedown/59fe7faf-081c-4d3b-8e2d-9e8b844dc1b0.png)

![204dcff5-4cb4-47bc-93ce-90444e2623fc](file:///C:/Users/DELL/Pictures/Typedown/204dcff5-4cb4-47bc-93ce-90444e2623fc.png)



### 战斗系统：

（1）配置表：防止硬编码带来的代码反复改动。



![01c4326b-6924-4fbd-b6a8-70ac63919170](file:///C:/Users/DELL/Pictures/Typedown/01c4326b-6924-4fbd-b6a8-70ac63919170.png)



![ec865dd1-8953-4ebc-b5c4-0853a31e085a](file:///C:/Users/DELL/Pictures/Typedown/ec865dd1-8953-4ebc-b5c4-0853a31e085a.png)



![9bf6415b-19f0-4226-9bd7-680d3d125e54](file:///C:/Users/DELL/Pictures/Typedown/9bf6415b-19f0-4226-9bd7-680d3d125e54.png)

![00bb0bed-e588-4968-83d6-1b1e78e77199](file:///C:/Users/DELL/Pictures/Typedown/00bb0bed-e588-4968-83d6-1b1e78e77199.png)



NetLua开发

![96988a19-a230-41aa-bc56-f7357eaa276a](file:///C:/Users/DELL/Pictures/Typedown/96988a19-a230-41aa-bc56-f7357eaa276a.png)

#### GameMainData主数据：

更新、存取玩家的各种模块的数据。

#### ConfigData配置文件：

UI模块

![9aed403a-a66e-42ca-8dcc-523eb8b2e3ef](file:///C:/Users/DELL/Pictures/Typedown/9aed403a-a66e-42ca-8dcc-523eb8b2e3ef.png)

![139e7468-1692-4115-9acf-b333914634df](file:///C:/Users/DELL/Pictures/Typedown/139e7468-1692-4115-9acf-b333914634df.png)

![a485a949-536d-4a13-9f6a-3e7e912d99aa](file:///C:/Users/DELL/Pictures/Typedown/a485a949-536d-4a13-9f6a-3e7e912d99aa.png)









![b3e170e2-9953-499f-a221-b527b400c85f](file:///C:/Users/DELL/Pictures/Typedown/b3e170e2-9953-499f-a221-b527b400c85f.png)
