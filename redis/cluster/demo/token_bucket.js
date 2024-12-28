const Redis = require("ioredis");

const redisMasters = [
  { host: "localhost", port: 7001 },
  { host: "localhost", port: 7002 },
  { host: "localhost", port: 7003 },
];

const redisCluster = new Redis.Cluster(redisMasters);

const rate_limit_lua =
  "local key = KEYS[1] " +
  "local rate = tonumber(ARGV[1]) " +
  "local capacity = tonumber(ARGV[2]) " +
  "local now = tonumber(ARGV[3]) " +
  "local requested = tonumber(ARGV[4]) " +
  'local last_tokens = tonumber(redis.call("GET", "{" .. key .. "}" .. ":tokens")) or capacity ' +
  'local last_refreshed = tonumber(redis.call("GET", "{" .. key .. "}" .. ":timestamp")) or now ' +
  "local delta = math.max(0, now - last_refreshed) * rate / 1000 " +
  "local filled_tokens = math.min(capacity, last_tokens + delta) " +
  "if filled_tokens < requested then " +
  "    return {0, filled_tokens} " +
  "else " +
  "    filled_tokens = filled_tokens - requested " +
  '    redis.call("SET", "{" .. key .. "}" .. ":tokens", filled_tokens) ' +
  '    redis.call("SET", "{" .. key .. "}" .. ":timestamp", now) ' +
  "    return {1, filled_tokens} " +
  "end";

async function acquireToken(key, rate, capacity, requested) {
  try {
    const now = new Date().getTime();
    const value = await redisCluster.eval(
      rate_limit_lua,
      1,
      key,
      rate.toString(),
      capacity.toString(),
      now.toString(),
      requested.toString()
    );
    return value[0] === 1;
  } catch (error) {
    console.error("Acquire token failed:", error);
    return false;
  }
}

async function example() {
  const key = "my_rate_limiter";
  const rate = 10;
  const capacity = 100;
  const requested = 1;

  let success_count = 0;
  let failed_count = 1;

  const intervalId = setInterval(async () => {
    const rlt = await acquireToken(key, rate, capacity, requested);
    if (rlt) {
      success_count = success_count + 1;
      console.log("Acquire token success");
    } else {
      failed_count = failed_count + 1;
      console.log("Acquire token failed");
    }
  }, 10);

  await new Promise((resolve) => setTimeout(resolve, 10000));
  clearInterval(intervalId);
  console.log("Success: ", success_count, " Failed: ", failed_count);
}

example().then(() => {
  process.exit(0);
});
