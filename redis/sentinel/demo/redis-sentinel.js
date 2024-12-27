const Redis = require("ioredis");
const { v4: uuidv4 } = require("uuid");

// 配置哨兵节点
const sentinelHosts = [
  { host: "localhost", port: 26379 },
  { host: "localhost", port: 26380 },
  { host: "localhost", port: 26381 },
];

// 创建哨兵客户端实例
const redis = new Redis({
  sentinels: sentinelHosts,
  name: "mymaster", // 哨兵配置中的主节点名称
  role: "master", // 连接到主节点进行写操作
});

// 创建只读客户端实例（连接到从节点）
const redisReadOnly = new Redis({
  sentinels: sentinelHosts,
  name: "mymaster", // 哨兵配置中的主节点名称
  role: "slave", // 连接到从节点进行读操作
});

// 写操作示例
async function writeExample() {
  try {
    await redis.set("key", "value");
    console.log("Write operation success");
  } catch (error) {
    console.error("Write operation failed:", error);
  }
}

// 读操作示例
async function readExample() {
  try {
    const value = await redisReadOnly.get("key");
    console.log("Read operation success, value:", value);
  } catch (error) {
    console.error("Read operation failed:", error);
  }
}

const LOCK_TTL = 5000; // 锁的初始过期时间，单位毫秒
const DOG_INTERVAL = 3000; // 看门狗定时器的触发间隔，单位毫秒

// 获取分布式锁
async function lock(lockKey, lockValue, ttl) {
  try {
    const value = await redis.set(lockKey, lockValue, "NX", "PX", ttl);
    console.log("Lock operation: ", value === "OK");
    return value === "OK";
  } catch (error) {
    console.error("Lock operation failed:", error);
    return false;
  }
}

// 释放分布式锁
async function unlock(lockKey, lockValue) {
  try {
    const script =
      'if redis.call("get", KEYS[1]) == ARGV[1] then return redis.call("del", KEYS[1]) else return 0 end';
    const result = await redis.eval(script, 1, lockKey, lockValue);
    console.log("Unlock operation: ", result === 1);
    return result === 1;
  } catch (error) {
    console.error("Unlock operation failed:", error);
    return false;
  }
}

// 看门狗
function startWatchdog(lockKey, lockValue) {
  console.log("Start Watchdog, key: ", lockKey, " value: ", lockValue);
  const intervalId = setInterval(async () => {
    const script =
      'if redis.call("get", KEYS[1]) == ARGV[1] then return redis.call("pexpire", KEYS[1], ARGV[2]) else return 0 end';
    const result = await redis.eval(script, 1, lockKey, lockValue, LOCK_TTL);
    if (result === 0) {
      clearInterval(intervalId);
    }
  }, DOG_INTERVAL);
  return intervalId;
}

// 使用uuid库: npm install uuid
// https://www.cnblogs.com/cloud-/p/18086034
// 一版由 时间戳+版本号+时钟序+节点标志
async function lockExample() {
  const lockKey = "test";
  const lockValue = uuidv4();

  const lockRlt = await lock(lockKey, lockValue, LOCK_TTL);
  if (lockRlt) {
    const intervalId = startWatchdog(lockKey, lockValue);
    try {
      // 模拟执行加锁的代码
      await new Promise((resolve) => setTimeout(resolve, 10000));
    } finally {
      clearInterval(intervalId);
      await unlock(lockKey, lockValue);
    }
  }
}

// 测试读写操作
async function testReadWrite() {
  await writeExample();
  await readExample();
  await lockExample();
}

testReadWrite().then(() => {
  process.exit(0);
});
