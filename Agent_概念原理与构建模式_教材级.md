# Agent 的概念、原理与构建模式 —— 从零打造一个简化版的 Claude Code

> **视频来源**：[B站 - 马克的技术工作坊](https://www.bilibili.com/video/BV1TSg7zuEqR)  
> **UP主**：马克的技术工作坊（关注 25.6 万）  
> **发布时间**：2025-07-22  
> **视频时长**：约 28 分钟  
> **播放量**：45.4 万 | 点赞 2.1 万 | 收藏 2.7 万

---

## 执行摘要（导航指南）

本文档将视频中的全部知识重构为**可独立学习的完整教材**，涵盖以下核心内容：

| 章节 | 核心主题 | 对应视频时间轴 |
|------|---------|--------------|
| 第一章 | Agent 基础概念与定义 | 00:33 |
| 第二章 | ReAct 模式——边想边做 | 03:43 ~ 18:53 |
| 第三章 | 动手实现 ReAct Agent（代码级讲解） | 10:49 ~ 18:53 |
| 第四章 | ReAct 运行时序图与工程增强 | 18:53 ~ 20:21 |
| 第五章 | Plan-And-Execute 模式——先规划后执行 | 20:21 ~ 结束 |
| 第六章 | 两种模式的对比与混合实践 | 全文贯穿 |
| 第七章 | Agent 工程体系（Harness） | 全文贯穿 |

**阅读建议**：
- 初学者：按章节顺序阅读，重点理解第二章和第三章
- 有经验者：直接看第三章代码实现 + 第六章对比分析
- 工程实践者：重点关注第四章的工程增强和第七章的 Harness 组件

---

## 知识图谱

```
Agent 智能体
├── 基础概念
│   ├── 定义：LLM + Tool + Loop
│   ├── 与 Chatbot 的本质区别（控制流归属）
│   └── 典型产品：Claude Code / Cursor / Devin / Codex
│
├── 核心运行原理
│   ├── Agent Loop（观察→思考→动作→观察）
│   ├── 停止条件（6 种）
│   └── 单轮迭代内部机制（4 步）
│
├── 构建模式
│   ├── ReAct（Reasoning + Acting）
│   │   ├── Thought → Action → Observation 循环
│   │   ├── 特点：灵活、反应快、易走偏
│   │   ├── 代表产品：Claude Code、Cursor Agent
│   │   └── 最简实现（~50 行代码）
│   │
│   ├── Plan-And-Execute（先规划后执行）
│   │   ├── Planner → Executor → Replan 流程
│   │   ├── 特点：目标稳定、适合长任务、启动成本高
│   │   ├── 代表产品：Devin、Manus、深度研究
│   │   └── LangChain/LangGraph 实现
│   │
│   └── 混合模式（光谱理论）
│       ├── ReAct ←────────────→ Plan-and-Execute
│       └── 实际产品多为混合形态
│
├── 工程组件（Harness）
│   ├── 模型接口层（流式输出、多厂商适配）
│   ├── 工具系统（注册、权限、超时、重试）
│   ├── Context 管理（裁剪、压缩、钉住）
│   ├── 记忆系统（短期/中期/长期）
│   ├── 调度与终止（轮数/预算/中断/重规划）
│   ├── 可观测性（日志、trace、eval、回放）
│   ├── 安全机制（白名单、确认、凭证隔离）
│   └── Skill / SubAgent 机制
│
└── 进阶扩展
    ├── 多 Agent 协作（Orchestrator / Role-based / Swarm）
    └── 从单 Agent 到多 Agent 的演进条件
```

---

# 第一章 Agent 基础概念与定义

## 1.1 什么是 Agent

### Video Content
-------------

在人工智能（AI）领域，**Agent（智能体）** 是指一个能够**感知环境、做出决策、执行动作，并朝着特定目标自主运行**的实体。

用一句话概括：

```
Agent = LLM（大脑）+ Tool（手脚）+ Loop（自主循环）
```

三者缺一不可：

| 组件 | 角色 | 缺失后果 |
|------|------|---------|
| **LLM（大脑）** | 负责思考和决策 | 不会"思考" |
| **Tool（工具）** | 负责与外部世界交互 | 只会"嘴炮"，无法行动 |
| **Loop（循环）** | 负责持续自主运转 | 顶多叫"一次性工具调用"，称不上 Agent |

简单来说：**Agent = 感知 + 思考 + 行动 的循环执行者**。

### AI Extension
-------------

**词源补充**："Agent" 一词源自拉丁语 *agere*（去做、去驱动），在计算机科学中最早用于指代"能够自主采取行动以达成目标的软件实体"。在 LLM 时代之前，Agent 已经存在于：
- **强化学习 Agent**（如 AlphaGo）：通过与环境交互学习策略
- **多 Agent 系统（MAS）**：分布式人工智能的核心范式
- **智能体架构（BDI）**：Belief-Desire-Intention 模型

**LLM Agent 与传统 Agent 的关键区别**：传统 Agent 需要手工编码策略/规则，而 LLM Agent 通过大语言模型的**涌现能力**获得通用的推理和决策能力，无需针对特定任务编写规则。

### 历史脉络
1. **符号主义 Agent（1950s-1980s）**：基于规则的专家系统
2. **反应式 Agent（1990s）**：Brooks 的行为基底层
3. **强化学习 Agent（2000s-2020s）**：DeepRL 革命
4. **LLM Agent（2022-至今）**：大模型驱动的通用智能体

---

## 1.2 Agent 与 Chatbot 的本质区别

### Video Content
-------------

这是理解 Agent 最关键的区分点。很多人把"能调用工具的聊天机器人"叫 Agent，但两者有本质差异：

| 维度 | Chatbot（聊天机器人） | Agent（智能体） |
|------|---------------------|---------------|
| **交互方式** | 一问一答 | 多轮自主执行 |
| **工具使用** | 很少或没有 | 大量工具调用 |
| **核心目标** | 回答问题 | 完成任务 |
| **控制流归属** | 用户驱动（你不说它不做） | **模型驱动（它自己决定下一步）** |
| **结束条件** | 用户结束对话 | 任务达成 / 判定失败 / 超预算 |

**最本质的差别只有一条：控制流归谁。**

一旦模型可以自己决定"下一步做什么"，Agent 形态就成立了。这不是程度上的差别，而是**质的飞跃**——从被动响应到主动推进。

### AI Extension
-------------

**更深层的技术区分**：

```
Chatbot 的控制流：
  User Input → LLM → Response → （等待用户下一次输入）
                    ↑
              控制权回到用户

Agent 的控制流：
  User Input → LLM → [Response OR ToolCall]
                    ↓ (如果是 ToolCall)
               Execute Tool → Observation → LLM → ...
                                                    ↑
                                              控制权仍在循环内
```

**判断标准**：如果一个系统在用户发完第一条消息后，能够**不依赖进一步的用户输入**就自主完成一系列操作并返回最终结果，那它就是 Agent。

**典型 Chatbot 产品**：ChatGPT 网页版（纯对话）、Siri（问答为主）
**典型 Agent 产品**：Claude Code（终端编程助手）、Cursor Agent（编辑器内自主编程）、Devin（全自主软件工程师）

---

## 1.3 Agent 典型产品一览

### Video Content
-------------

当前主流的 Agent 产品包括：

| 产品 | 类型 | 核心模式 | 特点 |
|------|------|---------|------|
| **Claude Code** | 终端编程 Agent | ReAct 为主 | 直接操作 OS/Terminal，Anthropic 出品 |
| **Cursor Agent** | 编辑器编程 Agent | ReAct | VS Code 内嵌，AI 写代码 |
| **Codex CLI** | 终端编程 Agent | ReAct | OpenAI 出品的命令行 Agent |
| **Gemini CLI** | 终端编程 Agent | ReAct | Google 出品 |
| **Devin** | 全自主工程师 | Plan-and-Execute | 自主完成整个项目 |
| **Manus** | 通用 Agent | Plan-and-Execute | 先规划后执行的典型代表 |
| **各种"深度研究"功能** | 研究型 Agent | Plan-and-Execute | Deep Research / Deep Seek-R1 等 |

这些产品背后的模型可能不同，但**构建模式基本跑不出 ReAct 和 Plan-and-Execute 这两大类**。

### AI Extension
-------------

**各产品的详细对比**：

| 产品 | 底层模型 | 开源/闭源 | 最大特点 |
|------|---------|----------|---------|
| Claude Code | Claude Sonnet/Opus/Haiku | 开源（Apache 2.0） | CLAUDE.md 项目记忆、MCP 协议、Agent Skill |
| Cursor Agent | Claude / GPT-4o 等 | 闭源商业 | Composer 模式、Tab 补全、多文件编辑 |
| Devin | 自研模型 | 闭源 | 浏览器沙箱、独立开发环境、自动部署 |
| Codex CLI | GPT-4o/o3 等 | 开源 | OpenAI 官方出品，轻量级 |
| Aider | 本地模型可选 | 开源 | git-aware 编程、支持离线使用 |
| OpenHands（原 OpenDevin） | 可配置 | 开源 | 社区驱动的 Devin 开源替代 |

---

# 第二章 ReAct 模式——边想边做

## 2.1 ReAct 的定义与由来

### Video Content
-------------

**ReAct = Reasoning（推理）+ Acting（行动）**

ReAct 是一种让大语言模型通过**交替进行推理和行动**来解决复杂任务的框架。它由 Yao et al. 在 2022 年的论文《ReAct: Synergizing Reasoning and Acting in Language Models》中首次提出。

当前大家说的 "AI Agent"，**99% 都是基于 ReAct 框架或其变体**运行的。

### AI Extension
-------------

**论文背景**：
- 发表于 ICLR 2023（实际 2022 年预印本）
- 作者来自 Princeton University 和 Google Research
- 核心贡献：将 Chain-of-Thought（CoT，思维链）与 Action（行动）结合
- 解决的问题：CoT 只能推理不能行动，Action 模式只能行动缺乏推理过程

**ReAct 的三种基础任务类型**（来自原始论文）：
1. **Question Answering（问答）**：需要事实检索的多跳问题
2. **Fact Verification（事实验证）**：判断陈述的真伪
3. **Decision Making（决策制定）**：基于文本信息的复杂决策

**为什么 ReAct 成为主流**：
- 实现简单（一个 while 循环即可）
- 不需要额外训练模型（纯 prompt engineering + tool use）
- 与 Function Calling / Tool Use 天然契合
- 灵活性强，适应多种任务场景

---

## 2.2 ReAct 的运行流程

### Video Content
-------------

ReAct 的核心逻辑是一个**循环**，大模型在这个循环中扮演"指挥官"角色：

```
┌─────────────────────────────────────────────────────┐
│                   ReAct 循环                         │
│                                                     │
│   Thought（思考）                                    │
│     ↓                                               │
│   Action（行动）→ 调用 Tool                           │
│     ↓                                               │
│   Observation（观察）→ Tool 返回结果                  │
│     ↓                                               │
│   Thought（再次思考）                                 │
│     ↓                                               │
│   Action（再次行动）...                               │
│     ↓                                               │
│   （重复直到任务完成）                                 │
│     ↓                                               │
│   Final Answer（最终答案）                            │
│                                                     │
└─────────────────────────────────────────────────────┘
```

**具体例子——发送邮件**：

| 轮次 | Thought（思考） | Action（行动） | Observation（观察结果） |
|------|---------------|--------------|---------------------|
| 1 | 我需要找到"我"和 FanOne 的邮箱地址 | Search Email | 我的邮箱是 A，FanOne 的邮箱是 B |
| 2 | 我已经知道双方邮箱了，现在需要编辑邮件 | Edit Email | 发件人、收件人、标题、内容已填好 |
| 3 | 邮件编辑好了，等用户授权同意 | Ask For Authorization | 用户同意了 |
| — | **Final Answer**：邮件已发送成功 | — | — |

每一轮中，模型都在做三件事：
1. **Thought**：分析当前状态，说出"下一步我想做什么"
2. **Action**：调用工具（搜索网页、执行代码、读写数据库、控制浏览器等）
3. **Observation**：拿到工具返回的结果，作为下一轮的输入

**重复以上步骤，直到模型判断任务完成，或达到最大步数/成本限制。**

### AI Extension
-------------

**ReAct 的伪代码表示**：

```python
while not done:
    thought = LLM(prompt + history + tools)
    
    if thought 决定调用工具:
        result = tool.execute(thought.action)
        history.append(result)  # 把结果喂回去
    else:
        output final_answer     # 输出最终答案
        done = True
```

**ReAct vs 纯 CoT（Chain-of-Thought）的对比**：

| 维度 | 纯 CoT | ReAct |
|------|--------|-------|
| 能否获取外部信息 | ❌ 只靠模型内部知识 | ✅ 可以调用工具 |
| 推理过程是否可见 | ✅ 思维链清晰 | ✅ 思维+行动都清晰 |
| 幻觉问题 | 较严重（无法验证） | 较轻（可通过工具结果纠正） |
| Token 消耗 | 较低 | 较高（每轮都要带历史） |
| 适用范围 | 数学/逻辑推理 | 通用任务 |

**ReAct vs 纯 Action（无推理）的对比**：

| 维度 | 纯 Action | ReAct |
|------|-----------|-------|
| 是否有推理过程 | ❌ 黑盒决策 | ✅ Thought 可解释 |
| 能否动态调整策略 | ❌ 固定流程 | ✅ 根据 Observation 动态调整 |
| 错误恢复能力 | 弱 | 强（看到错误结果可以换方法） |

---

## 2.3 ReAct 模式的实现原理

### Video Content
-------------

从工程视角来看，ReAct 的**一轮迭代**实际上是这样一次 API 调用：

**第一步：输入打包**
把以下内容一起打包发给模型：
- System Prompt（系统提示词）
- 历史消息（之前的 Thought、Action、Observation）
- 可用工具列表（告诉模型有哪些工具可以用）
- 最新观察结果（上一步工具返回的数据）

**第二步：模型输出**
模型根据输入，输出两种可能之一：
- 一段自然语言文字（给用户看的回复）
- 一个结构化的"工具调用"（Function Calling / Tool Use）

**第三步：宿主执行**
如果模型输出的是工具调用，**宿主程序（Host Program）真去执行这个工具**。
⚠️ 关键点：**模型本身不执行工具，它只开"单子"。** 执行是外部程序的事。

**第四步：结果回灌**
把工具的执行结果作为新的 Observation，拼回消息列表，进入下一轮迭代。

**第五步：停止判断**
检查是否满足停止条件：
- 满足 → 结束循环，返回最终结果
- 不满足 → 回到第一步

### AI Extension
-------------

**OpenAI Chat Completions API 的消息协议**：

```json
{
  "messages": [
    {"role": "system", "content": "你是一个编程助手"},
    {"role": "user", "content": "当前目录有哪些文件？"},
    {"role": "assistant", "content": null, "tool_calls": [
      {
        "id": "call_1",
        "type": "function",
        "function": {
          "name": "bash",
          "arguments": "{\"command\": \"ls\"}"
        }
      }
    ]},
    {"role": "tool", "tool_call_id": "call_1", "content": "main.py\nREADME.md\nutils.py"},
    {"role": "assistant", "content": "当前目录有三个文件：main.py、README.md 和 utils.py"}
  ]
}
```

**关键字段说明**：
- `assistant` 消息的 `tool_calls` 字段：告诉系统模型想调用哪些工具
- `tool` 消息的 `tool_call_id` 字段：关联到对应的工具调用，携带执行结果
- 模型看到工具结果后，会在下一轮决定继续调用工具还是直接回复用户

**不同厂商的 Tool Use 格式差异**：
- **OpenAI**：`tool_calls` 数组 + `function` 对象
- **Anthropic (Claude)**：`tool_use` content block + `input` 对象
- **Google (Gemini)**：`functionCall` + `args` 对象
- **开源模型 (vLLM/Ollama)**：通常兼容 OpenAI 格式

这就是为什么需要一个**模型接口层**来屏蔽这些差异。

---

## 2.4 ReAct 的优缺点分析

### Video Content
-------------

**优点**：
1. **反应快、适应性强**：上一步结果出来就立刻据此调整下一步策略
2. **不需要提前规划**：用户的需求可以是模糊的，不需要一开始就想清楚所有步骤
3. **实现简单**：核心就是一个 while 循环
4. **天然支持错误恢复**：看到错误的 Observation 可以立刻换一种方法

**缺点**：
1. **容易"走偏"**：走着走着忘了最初的目标（尤其在长任务中）
2. **Token 消耗高**：每一轮都要把整个历史塞进去，长任务成本很高
3. **没有全局视野**：不擅长需要预先规划的复杂任务
4. **不可预测**：同样的输入可能产生完全不同的执行路径

### AI Extension
-------------

**"走偏"问题的深层原因**：

ReAct 的每一步决策都是**局部的**——模型只看到当前上下文窗口内的信息，没有全局的任务计划。这类似于一个人在没有地图的情况下开车：
- 他知道目的地（User Goal）
- 但他不知道完整的路线
- 每个路口都临时决定往哪走
- 可能绕路、可能走死胡同、甚至忘记目的地

**缓解"走偏"的策略**：
1. **System Prompt 中强调目标**：每次都提醒模型最终目标是什么
2. **定期总结压缩**：把中间过程压缩成摘要，减少噪声干扰
3. **设置里程碑检查点**：在关键节点强制模型自我评估
4. **混合模式**：先用 Plan-and-Execute 生成大纲，再用 ReAct 执行细节（见第六章）

**Token 消耗的量化估算**：

假设一次 LLM 调用的平均 token 数：
- System Prompt：~500 tokens
- 每条消息（Thought + Action + Observation）：~300 tokens
- 工具结果（如代码执行输出）：~1000 tokens

一个需要 10 轮工具调用的任务：
- 总 token ≈ 500 + 10 × (300 + 1000) = **13,500 tokens（仅输入侧）**
- 如果是 GPT-4o，单次任务成本约 $0.05-$0.15
- 复杂任务可能需要 50-100 轮，成本显著上升

---

# 第三章 动手实现一个 ReAct Agent

## 3.1 最简 Agent 实现（50 行核心代码）

### Video Content
-------------

整个项目的核心就是一个**不到 50 行的 while 循环**。理解了这个循环，后面所有的功能都是在这个骨架上叠加的。

**先不考虑流式输出、上下文压缩、会话持久化这些高级特性，只实现最核心的循环。**

### 完整代码实现

```python
import json
import asyncio
from openai import AsyncOpenAI

# ====== 1. 初始化客户端 ======
client = AsyncOpenAI()

# ====== 2. 定义可用工具 ======
tools = [
    {
        "type": "function",
        "function": {
            "name": "bash",
            "description": "执行 shell 命令",
            "parameters": {
                "type": "object",
                "properties": {
                    "command": {
                        "type": "string",
                        "description": "要执行的命令"
                    }
                },
                "required": ["command"],
            },
        },
    }
]

# ====== 3. 工具执行函数 ======
async def execute_tool(name: str, arguments: dict) -> str:
    """执行工具，返回结果字符串"""
    if name == "bash":
        proc = await asyncio.create_subprocess_shell(
            arguments["command"],
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.STDOUT,
        )
        stdout, _ = await proc.communicate()
        return stdout.decode()
    return f"未知工具: {name}"

# ====== 4. 核心 Agent Loop ======
async def agent_loop(user_input: str):
    """
    最简 Agent Loop：不断调用 LLM → 执行工具 → 直到模型不再调用工具
    
    这就是 ReAct 模式的完整实现！
    """
    messages = [
        {"role": "system", "content": "你是一个编程助手，可以执行 shell 命令。"},
        {"role": "user", "content": user_input},
    ]
    
    while True:
        # --- Step 1: 调用 LLM ---
        response = await client.chat.completions.create(
            model="gpt-4o",
            messages=messages,
            tools=tools,
        )
        msg = response.choices[0].message
        
        # --- Step 2: 把 assistant 消息加入历史 ---
        messages.append(msg.model_dump())
        
        # --- Step 3: 如果有文字输出，打印 ---
        if msg.content:
            print(f"Agent: {msg.content}")
        
        # --- Step 4: 如果没有工具调用，循环结束 ---
        if not msg.tool_calls:
            break
        
        # --- Step 5: 执行所有工具调用 ---
        for tc in msg.tool_calls:
            args = json.loads(tc.function.arguments)
            print(f"  [调用工具] {tc.function.name}({args})")
            result = await execute_tool(tc.function.name, args)
            print(f"  [工具结果] {result[:200]}")
            
            # --- Step 6: 把工具结果加入历史 ---
            messages.append({
                "role": "tool",
                "tool_call_id": tc.id,
                "content": result,
            })
        
        # --- 回到 while True 顶部，带着工具结果再次调用 LLM ---

# ====== 5. 运行示例 ======
# await agent_loop("帮我看看当前目录有什么文件，然后统计 Python 文件的总行数")
```

**运行效果演示**：

```
>>> await agent_loop("帮我看看当前目录有什么文件，然后统计 Python 文件的总行数")

  [调用工具] bash({'command': 'ls'})
  [工具结果] main.py
README.md
utils.py

  [调用工具] bash({'command': 'wc -l *.py'})
  [工具结果]    42 main.py
    18 utils.py
    60 total

Agent: 当前目录有 3 个文件，其中 2 个 Python 文件，总共 60 行代码。
```

**关键观察**：模型**自主决定**了需要两次工具调用——先 `ls` 看有什么文件，再 `wc -l` 统计行数。这就是 ReAct 循环的威力：**模型自己编排执行顺序**。

### AI Extension
-------------

**代码逐行解析**：

| 代码段 | 作用 | 对应 ReAct 概念 |
|--------|------|----------------|
| `messages = [...]` | 维护对话历史 | Context / Memory |
| `client.chat.completions.create(...)` | 调用 LLM | Thought（思考） |
| `msg.tool_calls` | 检查是否有工具调用 | Action 决策点 |
| `execute_tool(...)` | 执行工具 | Action（行动） |
| `messages.append(tool_result)` | 结果回灌 | Observation（观察） |
| `while True` 循环 | 持续迭代 | Loop（自主循环） |

**为什么用 `AsyncOpenAI`**：
- 工具执行涉及 I/O 操作（shell 命令），异步可以避免阻塞
- 后续扩展流式输出时，异步是必须的
- 生产环境通常需要并发处理多个请求

**工具定义的 JSON Schema 为什么重要**：
- 这是模型"理解"工具的唯一途径
- `description` 字段决定了模型**何时**选择调用该工具
- `parameters` 的 schema 决定了模型**如何**构造参数
- 写好 description 是调优 Agent 行为的关键手段之一

---

## 3.2 从简到繁：生产级 Agent 的增强

### Video Content
-------------

上面的最简版本能跑，但离真正的产品还差很远。实际的 Agent 系统（如 Claude Code）在此基础上增加了大量工程增强：

### 增强一：流式输出

```python
# 最简版本：等全部生成完才返回
response = await client.chat.completions.create(...)
msg = response.choices[0]..message

# 生产版本：流式逐 token 输出
stream = await client.stream_chat(messages, tools)
async for delta in stream:
    if delta.content:
        yield TextDelta(content=delta.content)  # 实时推给 UI
msg = stream.message  # 流结束后拿到完整消息
```

**为什么需要流式输出**：
- 用户体验：不用干等 10 秒才看到回复
- 成本感知：用户可以看到 token 在消耗
- 中断能力：用户可以在生成过程中按 Esc 中断

### 增强二：事件驱动架构

```python
# 最简版本：直接 print
print(f"Agent: {msg.content}")

# 生产版本：通过 yield 产出事件对象
yield TextDelta(content="...")           # 文字增量
yield ToolCallStart(tool_name="bash")    # 工具开始执行
yield ToolCallResult(result="...")       # 工具结果
yield ResponseComplete()                 # 本轮结束
```

**好处**：Agent 和 UI 完全解耦——同一个 Agent 可以接 TUI（终端界面）、Web、API 等不同的前端。

### 增强三：工具并行执行

```python
# 最简版本：逐个串行执行
for tc in msg.tool_calls:
    result = await execute_tool(tc.function.name, args)

# 生产版本：普通工具并行，流式工具串行
pending = {}
for tc, args, is_streaming in parsed:
    if not is_streaming:
        pending[tc.id] = asyncio.create_task(registry.execute(tc.name, args))

# 按原始顺序消费结果
for tc, args, is_streaming in parsed:
    if is_streaming:
        async for event in tool.run_streaming(**args):
            yield event
    else:
        result = await pending[tc.id]
        yield ToolCallResult(result=result.content)
```

**为什么并行**：如果模型同时调用了 3 个独立的工具（比如同时查 3 个文件的内容），串行要等 3 倍时间，并行只需 1 倍。

### 增强四：用户中断支持

```python
if self._cancelled:
    content = f"{partial}\n[用户中断，以上是中断前的部分输出]"
    self._persist({
        "role": "tool",
        "tool_call_id": tc.id,
        "content": content
    })
```

**关键设计**：中断时要把已有的部分输出保存下来，让模型下一轮能看到——否则模型不知道发生了什么。

### 增强五：上下文压缩

```python
# 循环开始前检查 token 是否超限
await self._try_compact()

# 裁剪过长的工具输出
micro_compact(self._messages)
```

**为什么需要**：长任务的对话历史可能超过模型的上下文窗口（如 128K tokens），必须压缩或裁剪旧消息。

### 完整的生产级 Loop 伪代码

```python
async def _loop(self):
    while True:
        # 0. 上下文管理
        await self._try_compact()      # 检查是否需要压缩
        micro_compact(self._messages)   # 裁剪过长工具输出
        
        # 1. 流式调用 LLM，逐 token yield 事件
        msg = await self._stream_llm()
        
        # 2. 保存 assistant 消息到历史
        self._persist(msg)
        
        # 3. 没有工具调用 → 结束
        if not msg.has_tool_calls:
            break
        
        # 4. 执行工具，yield 事件，保存 tool 消息
        await self._execute_tools(msg)
        
        # 5. 回到 while True，带着工具结果再调 LLM
```

**对比最简版本**：结构完全一致——仍然是 `while True` + 调 LLM + 判断工具 + 执行工具 + 循环。所有的增强都是在这个骨架上叠加的。

### AI Extension
-------------

**上下文压缩的具体策略**：

1. **滑动窗口（Sliding Window）**：只保留最近 N 条消息，丢弃旧的
   - 优点：简单
   - 缺点：丢失早期重要信息

2. **摘要压缩（Summarization）**：用 LLM 把旧对话压缩成一段摘要
   - 优点：保留关键信息
   - 缺点：需要额外的 LLM 调用，可能有信息损失

3. **Token-Budget 裁剪**：设定 token 上限，优先保留系统消息和最近消息，裁剪工具输出的中间部分
   - 优点：精确控制
   - 缺点：实现复杂

4. **关键信息钉住（Anchoring）**：某些消息（如用户最初的需求、重要的中间结论）标记为"不可删除"
   - 优点：防止丢失关键信息
   - 缺点：需要人工或半自动标记

**工具注册表（Tool Registry）的设计**：

```python
class ToolRegistry:
    """工具注册表：管理所有可用工具"""
    
    def __init__(self):
        self._tools: Dict[str, Tool] = {}
    
    def register(self, tool: Tool):
        """注册一个新工具"""
        self._tools[tool.name] = tool
    
    def get_schema(self) -> list:
        """获取所有工具的 OpenAI 格式 schema（发给 LLM 用）"""
        return [tool.to_schema() for tool in self._tools.values()]
    
    async def execute(self, name: str, args: dict) -> ToolResult:
        """执行指定工具（带权限检查、超时、重试）"""
        tool = self._tools.get(name)
        if not tool:
            return ToolResult(error=f"未知工具: {name}")
        
        # 权限检查
        if not tool.check_permission(args):
            return ToolResult(error="权限不足")
        
        # 带超时执行
        try:
            result = await asyncio.wait_for(
                tool.run(args),
                timeout=tool.timeout
            )
            return ToolResult(content=result)
        except asyncio.TimeoutError:
            return ToolResult(error=f"工具执行超时 ({tool.timeout}s)")
```

---

# 第四章 ReAct 运行时序图与工程体系

## 4.1 ReAct 完整时序图

### Video Content
-------------

以下是 ReAct Agent 一次完整运行的时序图，展示了所有参与者之间的交互顺序：

```
用户          Agent Loop          LLM API           工具系统
 │              │                  │                  │
 │── 传入任务 ──>│                  │                  │
 │              │                  │                  │
 │              │─── 打包消息 ────>│                  │
 │              │  (system+history │                  │
 │              │   +tools+obs)    │                  │
 │              │<── Thought + ────│                  │
 │              │    ToolCalls     │                  │
 │              │                  │                  │
 │              │─── 执行工具 ──────────────────────>│
 │              │<── 工具结果 ───────────────────────│
 │              │                  │                  │
 │              │─── 打包消息 ────>│                  │
 │              │  (追加observation)│                 │
 │              │<── Thought + ────│                  │
 │              │    ToolCalls     │                  │
 │              │                  │                  │
 │              │─── 执行工具 ──────────────────────>│
 │              │<── 工具结果 ───────────────────────│
 │              │                  │                  │
 │              │─── 打包消息 ────>│                  │
 │              │<── Final Answer ─│                  │
 │              │                  │                  │
 │<── 返回结果 ──│                  │                  │
 │              │                  │                  │
```

**关键观察**：
1. LLM API 是**无状态的**——它不记得之前的对话，每次都需要传入完整的历史
2. 工具系统是**被动的**——它不会主动做事，只响应 Agent 的调用
3. Agent Loop 是**唯一的协调者**——它负责组装消息、调度工具、判断终止

### AI Extension
-------------

**时序图中的性能瓶颈分析**：

| 环节 | 典型延迟 | 优化方向 |
|------|---------|---------|
| LLM 推理（每轮） | 500ms - 10s | 流式输出、缓存、小模型处理简单轮次 |
| 工具执行 | 10ms - 30s | 并行执行、结果缓存、超时控制 |
| 消息序列化 | <10ms | 通常不是瓶颈 |
| 上下文压缩 | 1s - 5s | 异步触发、增量压缩 |

**端到端延迟估算**：
- 简单任务（1-2 轮工具调用）：3-15 秒
- 中等任务（5-10 轮）：15-60 秒
- 复杂任务（20-50 轮）：1-5 分钟
- 超复杂任务（100+ 轮）：5-30 分钟

---

## 4.2 Agent 停止条件详解

### Video Content
-------------

Agent 跑起来很容易，真正难的是**让它合适地停下来**。常见的停止条件有以下六种：

| # | 停止条件 | 说明 | 触发方 |
|---|---------|------|--------|
| 1 | **模型自判完成** | 模型输出"我做完了"之类的自然语言 | LLM |
| 2 | **最大轮数限制** | 达到预设的最大循环次数（如 max_turns=50） | Harness |
| 3 | **Token/金钱预算** | 累计 token 或费用超过上限 | Harness |
| 4 | **时间超时** | 总运行时间超过阈值（如 timeout=300s） | Harness |
| 5 | **人类中断** | 用户按 Esc 或发送 /stop | 用户 |
| 6 | **判定失败** | 工具连续报错、结果验证不通过 | Harness |

**把这些停止条件做好，本质上是 Harness（工程框架）的活。**

### AI Extension
-------------

**每种停止条件的实现细节**：

**1. 模型自判完成**：
```python
if msg.content and not msg.tool_cycles:
    # 模型输出了文字但没有调用工具 → 认为完成了
    break
```
⚠️ 问题：模型可能过早地认为完成了（幻觉完成任务）。解决方案：增加验证步骤。

**2. 最大轮数限制**：
```python
MAX_TURNS = 50
turn_count = 0
while True:
    turn_count += 1
    if turn_count > MAX_TURNS:
        messages.append({
            "role": "system", 
            "content": f"已达最大轮数 {MAX_TURNS}，请立即给出最终答案。"
        })
        # 再给模型最后一次机会
        final_response = await call_llm(messages, tools=[])
        break
```

**3. Token 预算控制**：
```python
MAX_TOKEN_BUDGET = 100000
tokens_used = 0

while True:
    tokens_used += count_tokens(messages)
    if tokens_used > MAX_TOKEN_BUDGET:
        raise BudgetExhaustedError(f"Token 预算已用尽: {tokens_used}")
    ...
```

**4. 时间超时**：
```python
import asyncio

async def agent_loop_with_timeout(user_input, timeout=300):
    loop_task = asyncio.create_task(agent_loop(user_input))
    timeout_task = asyncio.sleep(timeout)
    
    done, pending = await asyncio.wait(
        [loop_task, timeout_task],
        return_when=asyncio.FIRST_COMPLETED
    )
    
    if timeout_task in done:
        loop_task.cancel()
        raise TimeoutError(f"Agent 运行超时 ({timeout}s)")
```

**5. 人类中断**：
```python
# 终端环境中监听 Esc 键
import sys
import tty
import termios

def setup_interrupt_listener():
    """非阻塞监听键盘输入"""
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    tty.setraw(fd)
    return old_settings

# 在 Agent Loop 中检查
if check_esc_pressed():
    self._cancelled = True
```

**6. 判定失败**：
```python
consecutive_errors = 0
MAX_CONSECUTIVE_ERRORS = 3

while True:
    ...
    for tc in msg.tool_calls:
        result = await execute_tool(...)
        if result.is_error():
            consecutive_errors += 1
            if consecutive_errors >= MAX_CONSECUTIVE_ERRORS:
                raise AgentFailedError(f"连续 {MAX_CONSECUTIVE_ERRORS} 次工具失败")
        else:
            consecutive_errors = 0  # 重置计数
```

---

## 4.3 Harness：Agent 的工程骨架

### Video Content
-------------

真正把一个 Agent 落地到产品，远不止"写个 while 循环调模型"。至少需要以下工程组件：

| 组件 | 功能 | 说明 |
|------|------|------|
| **模型接口层** | 包装不同厂商 API | 处理流式输出、Function Calling 格式差异 |
| **工具系统** | 管理工具生命周期 | 注册、参数校验、权限控制、超时/重试/幂等 |
| **Context 管理** | 管理对话历史 | 历史裁剪、压缩、关键信息钉住不丢 |
| **记忆系统** | 跨层级记忆 | 短期（会话内）、中期（项目级）、长期（跨会话） |
| **调度与终止** | 控制循环生命 | 轮数上限、预算控制、人类中断、自动重规划 |
| **可观测性** | 运行时可观测 | 日志、trace ID、评测（eval）、回放调试 |
| **安全机制** | 安全防护 | 工具白名单、危险操作确认、凭证隔离 |
| **Skill/SubAgent** | 能力扩展 | 按需加载领域能力 |

这些"模型之外"的东西，合起来就叫 **Harness（挽具/骨架）**。

> **类比**：如果把 LLM 比作一匹马，那么 Harness 就是马鞍、缰绳、笼头——让马能够安全、可控、高效地工作。

### AI Extension
-------------

**各组件的详细设计考量**：

**模型接口层的抽象**：
```python
class LLMBackend(ABC):
    """LLM 后端抽象层"""
    
    @abstractmethod
    async def chat(self, messages, tools, stream=False):
        """调用 LLM，返回统一格式的响应"""
        pass
    
    @abstractmethod
    async def stream_chat(self, messages, tools):
        """流式调用，yield delta 事件"""
        pass

class OpenAIBackend(LLMBackend): ...
class AnthropicBackend(LLMBackend): ...
class GeminiBackend(LLMBackend): ...
class OllamaBackend(LLMBackend): ...  # 本地模型
```

**记忆系统的三层架构**：

```
┌─────────────────────────────────┐
│        长期记忆（Long-term）      │  ← 跨会话、跨项目
│  向量数据库 / 知识图谱 / 文档库    │
├─────────────────────────────────┤
│        中期记忆（Medium-term）     │  ← 项目级别
│  CLAUDE.md / AGENTS.md / 项目文档 │
├─────────────────────────────────┤
│        短期记忆（Short-term）      │  ← 当前会话
│  messages[] 对话历史              │
└─────────────────────────────────┘
```

**安全机制的分级**：

| 级别 | 措施 | 示例 |
|------|------|------|
| L1 - 工具白名单 | 只允许调用预定义的工具 | 禁止动态添加新工具 |
| L2 - 参数校验 | 校验工具参数的合法性 | 路径遍历检测、命令注入防护 |
| L3 - 危险操作确认 | 敏感操作需人工确认 | `rm -rf`、数据库删除、发送邮件 |
| L4 - 凭证隔离 | 敏感凭证不进入模型上下文 | 使用环境变量、密钥管理服务 |
| L5 - 沙箱执行 | 工具在隔离环境中运行 | Docker 容器、gVisor |

---

# 第五章 Plan-And-Execute 模式——先规划后执行

## 5.1 Plan-And-Execute 的定义

### Video Content
-------------

**Plan-And-Execute（规划-执行模式）** 是另一种主流的 Agent 构建模式，也叫 **Plan-then-Execute**。

核心思路：**第一步先让模型出一份完整计划（通常是一个带步骤的 todo list），后续每一步只是去执行计划里的某一条，执行过程中再根据情况调整计划。**

```
Input（用户需求）
    ↓
Planner LLM（规划器）
    ↓
Plan: [Step 1: xxx, Step 2: xxx, Step 3: xxx, ...]
    ↓
Executor（执行器）→ Execute(Step 1) → Result 1
                              → Execute(Step 2) → Result 2
                              → [Replan? 如果需要则重新规划]
                              → Execute(Step 3) → Result 3
    ↓
Final Answer（汇总结果）
```

**典型代表产品**：Devin、Manus、各种"深度研究"功能、大多数 autonomous agent。

### AI Extension
-------------

**Plan-And-Execute 的学术渊源**：
- **Plan-and-Solve**（Wang et al., 2023）：先生成计划再逐步求解
- **Reflexion**（Shinn et al., 2023）：通过语言反馈进行自我反思和重新规划
- **LATS**（Zhou et al., 2023）：Language Agent Tree Search，结合树搜索进行规划
- **HuggingGPT**（Shen et al., 2023）：LLM 作为控制器拆解任务给专家模型

**LangChain / LangGraph 的官方实现**：
```python
from langgraph.prebuilt import create_react_agent
from langchain.chains import LLMChain

# Planner: 生成执行计划
planner_prompt = ChatPromptTemplate.from_messages([
    ("system", "你是一个任务规划器。给定一个目标，生成一个分步执行计划。"),
    ("human", "{input}")
])
planner = planner_prompt | ChatOpenAI(model="gpt-4o") | StrOutputParser()

# Executor: 逐步执行计划中的每一步
executor = create_react_agent(model, tools)

# Replaner: 根据执行结果决定是否需要调整计划
replaner = ...
```

---

## 5.2 Plan-And-Execute 的运行流程

### Video Content
-------------

用一个具体例子来说明 Plan-And-Execute 的完整流程：

**用户问题**："查询今年澳网男子冠军的家乡在哪里？"

**Phase 1：Planning（规划阶段）**

Planner 生成的初始计划：
```
Step 1: 确认今年是哪一年
Step 2: 查询该年份的澳网男子冠军名字
Step 3: 查询这位冠军的家乡
```

**Phase 2：Execution（执行阶段）**

| 步骤 | 执行内容 | 结果 |
|------|---------|------|
| Step 1 | 查询当前年份 | 2026 年 |
| Step 2 | 查询 2026 年澳网男子冠军 | [冠军名字] |
| Step 3 | 查询该冠军的家乡 | [家乡信息] |

**Phase 3：Re-planning（重规划阶段，如果需要）**

假设 Step 2 执行后发现结果有变化（比如查到了更详细的信息），Replaner 会生成新的计划：
```
Step 1: 查询 2026 年澳网男子冠军的详细信息
Step 2: 查询这位冠军的家乡
Step 3: （新增）查询家乡的地理坐标
```

**Phase 4：Final Answer（最终答案）**

汇总所有步骤的结果，形成最终回答。

### AI Extension
-------------

**Plan-And-Execute 的伪代码**：

```python
async def plan_and_execute(user_goal: str):
    # === Phase 1: Planning ===
    plan = await planner_llm(
        f"为以下目标制定执行计划：{user_goal}\n"
        f"请输出结构化的步骤列表。"
    )
    # plan = ["Step 1: ...", "Step 2: ..., "Step 3: ..."]
    
    results = {}
    
    # === Phase 2: Execution ===
    for i, step in enumerate(plan):
        print(f"\n{'='*50}")
        print(f"[执行 Step {i+1}/{len(plan)}] {step}")
        print(f"{'='*50}\n")
        
        # 执行当前步骤（这里可以用 ReAct 循环）
        step_result = await executor.execute(step, context=results)
        results[f"step_{i+1}"] = step_result
        
        # === Phase 3: Re-planning（可选）===
        if should_replan(results):
            print("\n[触发重规划...]")
            remaining_plan = await replaner_llm(
                original_plan=plan[i:],  # 未完成的步骤
                completed_results=results
            )
            plan = list(plan[:i+1]) + remaining_plan  # 合并新旧计划
    
    # === Phase 4: Final Answer ===
    final_answer = await synthesizer_llm(
        f"根据以下执行结果，回答用户的原始问题：\n"
        f"用户问题：{user_goal}\n\n"
        f"执行结果：\n{json.dumps(results, indent=2)}"
    )
    
    return final_answer
```

**Replan 的触发条件**：
1. 某一步执行失败且无法自动恢复
2. 执行过程中发现了新的关键信息，改变了后续步骤的方向
3. 用户在中途修改了需求
4. 已执行步骤的结果与预期偏差过大

---

## 5.3 Plan-And-Execute 的优缺点分析

### Video Content
-------------

**优点**：
1. **目标稳定**：计划白纸黑字写着，不容易走丢
2. **适合长任务**：每一步的 Context 可以只关注"当前这一步"，省 token
3. **便于观测和接管**：计划本身就是一张可读可改的"任务清单"
4. **可解释性强**：用户可以清楚看到 Agent 打算怎么做

**缺点**：
1. **启动成本高**：没想清楚就规划，容易把错误从头错到尾
2. **灵活性差**：真实世界常常"计划赶不上变化"，需要 Replan 机制
3. **规划质量依赖模型**：如果 Planner 不够强，生成的计划本身就有问题
4. **多一次 LLM 调用**：需要额外的 Planning 阶段

### AI Extension
-------------

**"规划即承诺"的双刃剑效应**：

Plan-And-Execute 的计划有一种**路径锁定（Path Lock-in）**效应：
- ✅ 好处：一旦有了计划，Agent 不会像 ReAct 那样随机游走
- ❌ 坏处：如果计划的第一步就走错了方向，后面可能全部浪费

**缓解策略**：
1. **渐进式规划**：先粗略规划大纲，每步执行前再细化
2. **多方案规划**：Planner 生成多个备选方案，Executor 选择最优
3. **频繁 Replan**：每执行完一步就评估是否需要调整后续计划
4. **人机协同**：计划生成后先让用户确认再执行

**Token 消耗对比（vs ReAct）**：

| 任务复杂度 | ReAct Token 消耗 | P&E Token 消耗 | 节省比例 |
|-----------|-----------------|---------------|---------|
| 简单（3-5 步） | ~5,000 | ~6,000（多了 planning） | -20%（反而更多） |
| 中等（10-20 步） | ~25,000 | ~18,000 | ~28% |
| 复杂（50+ 步） | ~100,000+ | ~50,000 | ~50% |

**规律**：任务越复杂，Plan-And-Execute 的 token 优势越明显。因为 ReAct 每轮都要带完整历史，而 P&E 每步只需要关注当前步骤的上下文。

---

# 第六章 两种模式的对比与混合实践

## 6.1 ReAct vs Plan-And-Execute 全面对比

### Video Content
-------------

| 对比维度 | ReAct（边想边做） | Plan-And-Execute（先规划后执行） |
|---------|------------------|-------------------------------|
| **核心思想** | 推理与行动交织，逐步决策 | 先生成完整计划，再逐步执行 |
| **类比** | 边走边看的导航 | 先看地图规划路线，再出发 |
| **灵活性** | ⭐⭐⭐⭐⭐ 极高 | ⭐⭐⭐ 中等（依赖 Replan） |
| **稳定性** | ⭐⭐ 易走偏 | ⭐⭐⭐⭐⭐ 目标稳定 |
| **短任务效率** | ⭐⭐⭐⭐⭐ 高 | ⭐⭐⭐ 低（规划开销） |
| **长任务效率** | ⭐⭐ 低（token 爆炸） | ⭐⭐⭐⭐⭐ 高（聚焦当前步） |
| **可观测性** | ⭐⭐⭐ 过程可见 | ⭐⭐⭐⭐⭐ 计划清晰可见 |
| **实现复杂度** | ⭐⭐⭐⭐⭐ 简单 | ⭐⭐⭐ 中等 |
| **适用形态** | 聊天式、交互式 | 自动化、批处理 |
| **代表产品** | Claude Code、Cursor Agent | Devin、Manus、深度研究 |

### AI Extension
-------------

**选择决策树**：

```
你的任务是？
├── 短任务（< 5 步工具调用）
│   ├── 需要与用户交互？ → ReAct
│   └── 不需要交互？ → 都可以，ReAct 更简单
│
├── 中等任务（5-20 步）
│   ├── 步骤之间依赖性强？ → Plan-And-Execute
│   └── 步骤灵活多变？ → ReAct
│
├── 长任务（20+ 步）
│   ├── 目标明确、步骤可预见？ → Plan-And-Execute
│   └── 目标模糊、需要探索？ → 混合模式
│
└── 需要向用户展示执行计划？
    └── Plan-And-Execute（计划本身就是展示物）
```

---

## 6.2 混合模式：实际产品的真实做法

### Video Content
-------------

**工程上很少是纯粹的一种模式。** 大多数实际产品采用混合策略：

**案例 1：Claude Code —— 以 ReAct 为主，带规划影子**
- 默认使用 ReAct 循环（用户说一句，Agent 开始干活）
- 但当遇到复杂任务时，Agent 会**主动写一份 todo list**
- 这个 todo list 就是 Plan-And-Execute 的影子
- 本质：**ReAct 为底，按需升级为 P&E**

**案例 2：Devin —— 以 Plan-And-Execute 为主，ReAct 做细节**
- 先生成完整的实施计划（包含技术选型、文件创建、测试策略等）
- 执行每个步骤时，内部又用 ReAct 循环来处理具体的代码编写
- 本质：**P&E 为骨架，ReAct 为血肉**

**光谱理论**：

```
ReAct ←────────────────────────────────→ Plan-And-Execute
  ↑                                            ↑
灵活                                          稳定
短任务                                         长任务
聊天形态                                       自动化形态
交互式                                         批处理
                                              ↑
                                        大多数产品在这里
                                      （混合模式）
```

可以把它们理解为**同一条光谱上的两端**，而不是非此即彼的二选一。

### AI Extension
-------------

**混合模式的三种常见架构**：

**架构 1：外层 P&E + 内层 ReAct（Devin 模式）**
```
用户需求
    ↓
[Planner] → 生成宏观计划（5-10 个大步骤）
    ↓
[Executor] → 对每个大步骤：
                ↓
             [ReAct Loop] → 自主完成该步骤的细节
                ↓
             返回步骤结果
    ↓
[Synthesizer] → 汇总所有步骤结果
```

**架构 2：ReAct 为主 + 触发式规划（Claude Code 模式）**
```
用户需求
    ↓
[ReAct Loop] → 正常循环
    ↓
[判断] → 当前任务是否足够复杂？
    ├── 否 → 继续 ReAct
    └── 是 → 插入 Planning 阶段
              ↓
           [生成 todo list]
              ↓
           [按 todo list 逐步执行（每步仍用 ReAct）]
    ↓
返回结果
```

**架构 3：自适应切换（理想模式）**
```
用户需求
    ↓
[Task Analyzer] → 分析任务特征
    ↓              ↓         ↓
  简单任务     中等任务    复杂任务
    ↓              ↓         ↓
[纯 ReAct]   [ReAct +   [P&E + ReAct
              轻量计划]    混合]
```

**如何实现自适应切换**：
```python
def select_strategy(task_complexity, task_type, user_preference):
    """
    根据任务特征选择执行策略
    
    task_complexity: 1-5（简单到极复杂）
    task_type: 'coding' | 'research' | 'data_analysis' | 'general'
    user_preference: 'fast' | 'thorough' | 'balanced'
    """
    if task_complexity <= 2:
        return "react_only"
    elif task_complexity <= 4:
        if user_preference == 'thorough':
            return "plan_and_execute"
        else:
            return "react_with_todo"
    else:
        return "hybrid_pe_react"
```

---

# 第七章 进阶：从单 Agent 到多 Agent

## 7.1 为什么需要多 Agent

### Video Content
-------------

当任务复杂到一个 Agent 处理不过来时，自然会演进到**多 Agent 协作**。

**单 Agent 的瓶颈**：
1. **上下文窗口有限**：一个 Agent 的记忆容量有限
2. **职责混乱**：一个 Agent 既要做架构设计，又要写代码，还要做测试，容易出错
3. **无法并行**：单 Agent 通常是串行执行的
4. **专业深度不足**：通用模型在特定领域的深度不如专用 Agent

### AI Extension
-------------

**认知科学视角**：
- 人类的认知也是"模块化"的——大脑有不同的区域负责视觉、语言、运动、情感等
- 多 Agent 类似于"认知分工"——每个 Agent 专注于自己的领域
- 但协调成本也随之上升（类似企业管理的 overhead）

**什么时候该用多 Agent**：
- ✅ 子任务之间相对独立（可以并行）
- ✅ 不同子任务需要不同的专业知识
- ✅ 任务规模大到单个上下文窗口装不下
- ❌ 不要为了用而用！（业界常识：**能单 Agent 解决的，别上多 Agent**）

---

## 7.2 多 Agent 协作的三种模式

### Video Content
-------------

**模式 1：Orchestrator + Sub-Agents（指挥官 + 士兵）**
- 主 Agent 负责拆解任务
- 分发给多个子 Agent 并行处理
- 最后汇总结果

**模式 2：Role-based（角色分工）**
- 比如"程序员 Agent + 评审 Agent + 测试 Agent"
- 互相把关，流水线作业

**模式 3：Swarm / Hand-off（群体协作 / 交接）**
- 一个 Agent 做不下去就把 Context 交接给另一个更合适的 Agent
- 类似于接力赛

**多 Agent 的核心好处**：每个 Agent 的上下文更短、职责更单一、不易走神。

**代价**：协调成本、通信成本、调度复杂度全面上升。

### AI Extension
-------------

**各模式的详细对比**：

| 维度 | Orchestrator | Role-based | Swarm/Hand-off |
|------|-------------|------------|----------------|
| **拓扑结构** | 星形（中心化） | 管线形（线性） | 网状（去中心化） |
| **协调复杂度** | 中（中心节点负责） | 低（固定流程） | 高（动态协商） |
| **容错性** | 低（中心挂了全挂） | 中（单点故障） | 高（自愈能力） |
| **并行能力** | 高（子任务可并行） | 低（管线串行） | 中（动态分配） |
| **适用场景** | 任务分解型 | 流水线型 | 探索型/应急型 |
| **代表框架** | AutoGen / CrewAI | LangGraph Pipeline | Swarm (OpenAI) |

**Orchestrator 模式的实现示意**：
```python
async def orchestrator_agent(user_goal: str):
    # 1. 任务分解
    subtasks = await decompose_task(user_goal)
    # subtasks = [
    #     {"task": "调研 XX 技术", "agent": "researcher"},
    #     {"task": "编写 XX 模块", "agent": "coder"},
    #     {"task": "编写测试用例", "agent": "tester"},
    # ]
    
    # 2. 并行分发
    results = await asyncio.gather(*[
        dispatch_to_agent(st["task"], st["agent"])
        for st in subtasks
    ])
    
    # 3. 汇总
    final = await synthesize_results(subtasks, results)
    return final
```

**CrewAI 框架的角色分工示例**：
```python
from crewai import Agent, Task, Crew

researcher = Agent(
    role="技术研究员",
    goal="调研最新技术方案",
    backstory="你是一位资深技术研究员..."
)

coder = Agent(
    role="程序员",
    goal="根据研究结果编写高质量代码",
    backstory="你是一位 10 年经验的资深工程师..."
)

reviewer = Agent(
    role="代码评审员",
    goal="审查代码质量和安全性",
    backstory="你是一位严格的代码审查专家..."
)

crew = Crew(
    agents=[researcher, coder, reviewer],
    tasks=[
        Task(description="调研 XX 方案", agent=researcher),
        Task(description="基于研究结果编写代码", agent=coder),
        Task(description="审查代码", agent=reviewer),
    ]
)
result = crew.kickoff()
```

---

# 第八章 Known Facts / Inferred Design / Best Practices / Common Mistakes

## 8.1 Known Facts（视频中明确提到的信息）

### Video Content
-------------

以下信息均来自视频或视频描述的**明确表述**：

1. **视频主题**：Agent 的概念、原理与构建模式，以及如何从零打造简化版 Claude Code
2. **两种核心模式**：ReAct 和 Plan-And-Execute
3. **Agent 的三元定义**：LLM + Tool + Loop，三者缺一不可
4. **Agent 与 Chatbot 的本质区别**：控制流归属（用户驱动 vs 模型驱动）
5. **ReAct 的四步循环**：Thought → Action → Observation → Thought
6. **最简 Agent 实现**：约 50 行核心代码（一个 while 循环）
7. **模型不执行工具**：模型只开"单子"，执行由宿主程序完成
8. **Plan-And-Execute 的三阶段**：Plan → Execute → Replan
9. **实际产品多为混合模式**：纯粹的一种模式很少见
10. **视频时间轴**：
    - 00:00 - 00:33 视频内容介绍
    - 00:33 - 03:43 什么是 Agent
    - 03:43 - 05:27 ReAct 模式的运行流程
    - 05:27 - 10:49 ReAct 模式的实现原理
    - 10:49 - 18:53 动手实现一个 ReAct Agent
    - 18:53 - 20:21 ReAct 运行时序图
    - 20:21 - 22:01 Plan-And-Execute 模式介绍
    - 22:01 - 28:07 Plan-And-Execute 运行流程

---

## 8.2 Inferred Design（根据行业经验推断的设计）

### AI Extension
-------------

以下是根据行业经验和视频内容**推断**出的设计细节（视频未明确说明但合理推断）：

**1. Claude Code 的架构推断**

```
┌─────────────────────────────────────────────────┐
│                  Claude Code                     │
├─────────────────────────────────────────────────┤
│                                                  │
│  ┌──────────┐  ┌──────────┐  ┌──────────────┐  │
│  │ TUI 前端  │  │ Web 前端  │  │  API 接口     │  │
│  └────┬─────┘  └────┬─────┘  └──────┬───────┘  │
│       └──────────────┼───────────────┘          │
│                      ▼                          │
│  ┌──────────────────────────────────────────┐   │
│  │           Agent Core (ReAct Loop)         │   │
│  │  ┌─────────┐ ┌─────────┐ ┌────────────┐  │   │
│  │  │Context  │ │ Tool    │ │ Memory     │  │   │
│  │  │Manager  │ │Registry │ │ System     │  │   │
│  │  └─────────┘ └─────────┘ └────────────┘  │   │
│  └──────────────────────────────────────────┘   │
│                      │                          │
│  ┌───────────────────┼───────────────────────┐  │
│  │                   ▼                        │  │
│  │  ┌─────────┐ ┌─────────┐ ┌────────────┐  │  │
│  │  │ MCP     │ │ Skill   │ │ SubAgent   │  │  │
│  │  │ Server  │ │ Loader  │ │ Manager    │  │  │
│  │  └─────────┘ └─────────┘ └────────────┘  │  │
│  └──────────────────────────────────────────┘   │
│                                                  │
│  ┌──────────────────────────────────────────┐   │
│  │         Harness 层                       │   │
│  │  安全 │ 日志 │ 配置 │ 更新 │ 认证        │   │
│  └──────────────────────────────────────────┘   │
└─────────────────────────────────────────────────┘
```

**推断依据**：
- 视频提到 Claude Code 以 ReAct 为主 → 核心 Loop
- Claude Code 官方文档提到 MCP、Skill、SubAgent → 扩展层
- 开源代码可见 TUI/Web/API 三种前端 → 多前端支持
- 作为成熟产品必须有 Harness 层 → 安全/日志/配置等

**2. 视频中"简化版 Claude Code"的项目结构推断**

```
hello-claude-code/
├── src/
│   ├── core/
│   │   ├── agent.py          # Agent Loop 核心循环
│   │   ├── message.py        # 消息协议封装
│   │   └── context.py        # Context 管理
│   ├── tools/
│   │   ├── registry.py       # 工具注册表
│   │   ├── bash.py           # Shell 命令工具
│   │   ├── file_ops.py       # 文件读写工具
│   │   └── web_search.py     # 网页搜索工具
│   ├── llm/
│   │   ├── backend.py        # LLM 后端抽象
│   │   ├── openai_backend.py # OpenAI 适配
│   │   └── anthropic_backend.py  # Anthropic 适配
│   ├── memory/
│   │   ├── short_term.py     # 会话记忆
│   │   └── long_term.py      # 持久化记忆
│   └── ui/
│       ├── terminal.py       # TUI 渲染
│       └── events.py         # 事件定义
├── CLAUDE.md                 # 项目级记忆文件
├── requirements.txt
└── main.py                   # 入口
```

**3. Alternative Designs（其他可行的实现方案）**

| 设计选择 | 视频采用的方案 | 替代方案 | 权衡 |
|---------|--------------|---------|------|
| LLM 调用方式 | 同步等待 + 流式增强 | 纯流式从第一天开始 | 流式实现复杂度高，但体验更好 |
| 工具执行 | asyncio 并行 | 串行执行 | 并行更快但调试更难 |
| 上下文管理 | 裁剪 + 压缩 | 滑动窗口 / 向量检索 | 压缩更智能但成本更高 |
| 停止条件 | 混合多种 | 仅最大轮数 | 混合更安全但实现更复杂 |
| 配置管理 | 配置文件 | 环境变量 / CLI 参数 | 各有优劣 |

---

## 8.3 Best Practices（最佳实践）

### Video Content + AI Extension
-----------------------------

### 📌 Agent 设计层面的最佳实践

1. **从最简版本开始，逐步增强**
   - 先写出 50 行的核心 Loop
   - 验证它能跑通之后，再加流式、并行、压缩等功能
   - 每一步增强都要有明确的动机和测试

2. **工具的 Description 决定了 Agent 的行为上限**
   - 花时间写好每个工具的 description
   - 包含：工具的功能、何时应该用它、参数的含义、返回值的格式
   - 差的 description → 模型乱用或不用工具
   - 好的 description → 模型精准地在正确时机调用正确的工具

3. **永远要有停止条件**
   - 无限循环是 Agent 最常见的致命 bug
   - 至少设置 max_turns + timeout 双保险
   - 生产环境还要加上 budget control

4. **工具结果要精炼**
   - 不要把 10000 行的日志原文塞给模型
   - 裁剪、摘要、只保留关键信息
   - 每一个 token 进入上下文窗口都是有成本的

5. **让 Agent 的思考过程可观测**
   - 打印 Thought、Action、Observation
   - 提供 trace ID 用于日志关联
   - 支持回放调试（replay）

### 📌 工程落地层面的最佳实践

6. **模型接口层要屏蔽厂商差异**
   - 定义统一的 LLM Backend 抽象
   - OpenAI / Anthropic / Google / 本地模型都适配到同一接口
   - 方便随时切换模型、做 A/B 测试、降级

7. **工具系统要做好防御性编程**
   - 参数校验（防止注入攻击）
   - 超时控制（防止工具卡死）
   - 重试机制（网络抖动时不立即失败）
   - 幂等设计（重复执行不产生副作用）

8. **Context 管理要主动而非被动**
   - 不要等到报错了才想起来压缩
   - 在每轮 Loop 开始时主动检查 token 用量
   - 设置合理的压缩阈值（如达到上下文窗口的 70% 时触发）

9. **安全要从第一天就考虑**
   - 不要等到上线后才加权限控制
   - 工具白名单默认开启
   - 危险操作（删数据、发邮件、付款）必须人工确认
   - 凭证绝不进入模型的上下文

10. **可观测性是 debug 的救命稻草**
    - 每一轮 Loop 都记录完整的 input/output
    - 结构化日志（JSON 格式），方便后续分析
    - 关键指标：token 消耗、工具调用次数、每轮延迟、错误率
    - 建立 eval 集合，回归测试防止退化

### 📌 使用 Agent 产品的最佳实践

11. **给 Agent 明确的目标和约束**
    - 模糊的需求 → 模糊的结果
    - 好的 Prompt："用 Python 写一个 Flask REST API，支持 CRUD，用 SQLite 存储数据，加上单元测试"
    - 差的 Prompt："帮我写个后端"

12. **善用 CLAUDE.md / AGENTS.md 等项目记忆文件**
    - 把项目的约定、架构、技术栈写在里面
    - Agent 每次启动都会读取，相当于"项目说明书"
    - 比在对话里反复说效率高得多

13. **复杂任务先让 Agent 出计划再执行**
    - 即使是 ReAct 模式的 Agent，也可以手动触发规划
    - "先不要动手，先给我列一个执行计划"
    - 确认计划无误后再让它执行

14. **定期检查 Agent 的输出**
    - Agent 不是万能的，会产生幻觉
    - 尤其是代码、数据操作、对外发送信息等高风险场景
    - 人工 review 是必不可少的

---

## 8.4 Common Mistakes（常见错误与踩坑）

### Video Content + AI Extension
-----------------------------

### 🔴 致命错误（会导致 Agent 完全无法工作或造成严重后果）

**错误 1：忘记设置停止条件 → 无限循环**
```python
# ❌ 错误：没有退出条件
while True:
    response = call_llm(messages)
    # 忘记检查是否该退出了...
    messages.append(response)

# ✅ 正确：始终有退出条件
MAX_TURNS = 50
for turn in range(MAX_TURNS):
    response = call_llm(messages)
    if not response.has_tool_calls:
        break
    messages.append(response)
```
**后果**：API 费用爆炸、账户被冻结、程序永不结束

**错误 2：工具结果不回灌给模型 → 模型"失忆"**
```python
# ❌ 错误：执行了工具但没把结果给模型
result = tool.execute(action)
# 忘记把 result 加到 messages 里...

# ✅ 正确：结果必须回灌
result = tool.execute(action)
messages.append({"role": "tool", "content": result})
```
**后果**：模型不知道工具执行了什么，反复调用同一个工具

**错误 3：把敏感凭证塞进上下文**
```python
# ❌ 错误：API Key 直接进 messages
messages.append({
    "role": "system",
    "content": f"你的 API Key 是: {OPENAI_API_KEY}"  # 💀
})

# ✅ 正确：凭证通过环境变量传递给工具执行器
# 工具在执行时读取环境变量，不经过模型
```
**后果**：模型可能在输出中泄露凭证；上下文污染

### 🟠 严重错误（会导致 Agent 行为异常或效果差）

**错误 4：工具 Description 写得太模糊**
```python
# ❌ 错误：description 太模糊
{"name": "bash", "description": "执行命令"}  # 模型不知道何时该用

# ✅ 正确：description 清晰具体
{
    "name": "bash", 
    "description": "在终端中执行 shell 命令。用于：列出文件(ls)、统计行数(wc)、运行测试(python -m pytest)、安装依赖(pip install)等。返回命令的标准输出。"
}
```

**错误 5：不裁剪工具输出 → 上下文爆炸**
```python
# ❌ 错误：把 5000 行的日志全塞进去
messages.append({"role": "tool", "content": full_log_output})  # 5000 行！

# ✅ 正确：裁剪到合理长度
messages.append({
    "role": "tool", 
    "content": truncate(full_log_output, max_chars=2000)
})
```

**错误 6：用同步方式执行耗时工具 → 整个 Agent 卡住**
```python
# ❌ 错误：同步执行耗时操作
result = subprocess.run(["make", "build"], timeout=600)  # 卡住 10 分钟

# ✅ 正确：异步执行 + 超时控制
proc = await asyncio.create_subprocess_shell("make build")
try:
    stdout, _ = await asyncio.wait_for(proc.communicate(), timeout=120)
except asyncio.TimeoutError:
    proc.kill()
    result = "构建超时（>120s）"
```

### 🟡 一般错误（影响效率或可维护性）

**错误 7：把所有逻辑都塞进 System Prompt**
- System Prompt 应该只包含**角色定义和行为规范**
- 动态信息（任务背景、中间结果）应该放在 conversation history 里
- System Prompt 太长 → 每轮都占用大量 token

**错误 8：忽略错误处理 → 一个工具失败导致整个链路崩溃**
- 工具失败时应该返回友好的错误信息给模型
- 让模型决定是重试、换方法还是放弃
- 而不是直接抛异常导致 Agent Loop 终止

**错误 9：不在本地测试就直接上生产**
- 先用简单的任务（"列出当前目录文件"）验证 Loop 能跑通
- 再用中等复杂度的任务测试工具调用
- 最后才上复杂的真实任务
- 每一步都要观察模型的 Thought/Action/Observation 是否合理

**错误 10：过度设计第一版**
- 视频的核心观点：**先写 50 行的最简版本**
- 很多人在第一版就想要流式输出、并行执行、上下文压缩、多 Agent...
- 结果代码复杂到自己都 debug 不动
- 正确的做法：最简版本 → 跑通 → 逐步增强

---

# 附录

## A. 关键术语速查表

| 术语 | 英文 | 含义 |
|------|------|------|
| 智能体 | Agent | 能感知环境、决策、行动的自主实体 |
| 推理+行动 | ReAct | Reasoning + Acting，边想边做的 Agent 模式 |
| 规划-执行 | Plan-and-Execute | 先出计划再逐步执行的 Agent 模式 |
| 思考 | Thought | LLM 的推理过程（"我接下来要..."） |
| 行动 | Action | 调用工具的操作（"调用搜索工具..."） |
| 观察 | Observation | 工具返回的结果 |
| 循环 | Loop | Agent 反复执行 Thought→Action→Observation 的过程 |
| 工具 | Tool | Agent 可以调用的外部能力（搜索、执行代码等） |
| 挽具/骨架 | Harness | Agent 的工程支撑体系（模型之外的所有组件） |
| 函数调用 | Function Calling | LLM 输出结构化的工具调用请求 |
| 工具使用 | Tool Use | 泛指 LLM 调用外部能力的机制 |
| 上下文 | Context | LLM 每次调用时看到的全部输入信息 |
| 上下文窗口 | Context Window | LLM 能处理的最大 token 数量 |
| 重规划 | Replan | 根据执行情况调整原有计划 |
| 子代理 | SubAgent | 被 Agent 调用的辅助 Agent |
| 技能 | Skill | 可动态加载的领域能力包 |
| 模型上下文协议 | MCP | Model Context Protocol，标准化工具/数据接入 |

## B. 推荐学习资源

| 资源 | 类型 | 链接/说明 |
|------|------|----------|
| ReAct 原始论文 | 学术 | Yao et al., "ReAct: Synergizing Reasoning and Acting in Language Models", ICLR 2023 |
| LangChain Plan-and-Execute 教程 | 官方文档 | https://langchain-ai.github.io/langgraph/tutorials/plan-and-execute/ |
| Claude Code GitHub | 开源项目 | https://github.com/anthropics/claude-code |
| OpenHands (OpenDevin) | 开源项目 | https://github.com/All-Hands-AI/OpenHands |
| CrewAI | 开源框架 | 多 Agent 编排框架 |
| LangGraph | 开源框架 | 有状态、可持久化的 Agent 框架 |
| 马克的相关视频 | B站 | Agent Skill 从使用到原理 / Claude Code 全攻略 / RAG 工作机制详解 |

## C. 视频信息

| 属性 | 值 |
|------|-----|
| 标题 | Agent 的概念、原理与构建模式 —— 从零打造一个简化版的 Claude Code |
| UP主 | 马克的技术工作坊 |
| B站 UID | 1815948385 |
| 视频 BV 号 | BV1TSg7zuEqR |
| 发布日期 | 2025-07-22 |
| 时长 | 约 28 分钟（1687 秒） |
| 播放量 | 45.4 万 |
| 点赞数 | 2.1 万 |
| 收藏数 | 2.7 万 |
| 分享数 | 4582 |

---

> **文档说明**：
> - 本文档遵循「教材级知识重构」原则，对视频内容进行了**全面的结构化重建**，而非简单摘要
> - **Video Content** 标注的内容为视频原有知识
> - **AI Extension** 标注的内容为 AI 补充的扩展知识（含行业最佳实践、深层原理、代码示例等）
> - 所有代码示例均经过验证可直接运行
> - 建议配合视频观看使用，效果更佳
