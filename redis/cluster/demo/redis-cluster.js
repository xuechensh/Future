const Redis = require("ioredis");

const redisMasters = [
  { host: "localhost", port: 7001 },
  { host: "localhost", port: 7002 },
  { host: "localhost", port: 7003 },
];

const redisCluster = new Redis.Cluster(redisMasters);

async function writeExample() {
  try {
    await redisCluster.set("key", "value");
    console.log("Write operation success");
  } catch (error) {
    console.error("Write operation failed:", error);
  }
}

async function readExample() {
  try {
    const value = await redisCluster.get("key");
    console.log("Read operation success, value:", value);
  } catch (error) {
    console.error("Read operation failed:", error);
  }
}

async function example() {
  await writeExample();
  await readExample();
}

example().then(() => {
  process.exit(0);
});
