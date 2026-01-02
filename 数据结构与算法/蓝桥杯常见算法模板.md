**前缀和**、**差分**、**二分法**、**贪心算法**、**分治算法**、**KMP 算法**。
[[动态规划常见模版]]

```java
public class knapsack_problems {
    public static class KnapsackProblemsNonRolling {

        // 0 - 1 背包问题（非滚动数组）
        public static int zeroOneKnapsack(int[] weights, int[] values, int capacity) {
            int n = weights.length;
            int[][] dp = new int[n + 1][capacity + 1];

            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= capacity; j++) {
                    if (j < weights[i - 1]) {
                        dp[i][j] = dp[i - 1][j];
                    } else {
                        dp[i][j] = Math.max(dp[i - 1][j], dp[i - 1][j - weights[i - 1]] + values[i - 1]);
                    }
                }
            }
            return dp[n][capacity];
        }

        // 完全背包问题（非滚动数组）
        public static int completeKnapsack(int[] weights, int[] values, int capacity) {
            int n = weights.length;
            int[][] dp = new int[n + 1][capacity + 1];

            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= capacity; j++) {
                    if (j < weights[i - 1]) {
                        dp[i][j] = dp[i - 1][j];
                    } else {
                        dp[i][j] = Math.max(dp[i][j], dp[i][j - weights[i - 1]] + values[i - 1]);
                    }
                }
            }
            return dp[n][capacity];
        }

        // 多重背包问题（非滚动数组）
        public static int multipleKnapsack(int[] weights, int[] values, int[] counts, int capacity) {
            int n = weights.length;
            int[][] dp = new int[n + 1][capacity + 1];

            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= capacity; j++) {
                    dp[i][j] = dp[i - 1][j];
                    for (int k = 1; k <= counts[i - 1] && k * weights[i - 1] <= j; k++) {
                        dp[i][j] = Math.max(dp[i][j], dp[i - 1][j - k * weights[i - 1]] + k * values[i - 1]);
                    }
                }
            }
            return dp[n][capacity];
        }
    }

    public static class KnapsackProblemsRolling {

        // 0 - 1 背包问题（滚动数组优化）
        public static int zeroOneKnapsack(int[] weights, int[] values, int capacity) {
            int n = weights.length;
            int[] dp = new int[capacity + 1];

            for (int i = 0; i < n; i++) {
                for (int j = capacity; j >= weights[i]; j--) {
                    dp[j] = Math.max(dp[j], dp[j - weights[i]] + values[i]);
                }
            }
            return dp[capacity];
        }

        // 完全背包问题（滚动数组优化）
        public static int completeKnapsack(int[] weights, int[] values, int capacity) {
            int n = weights.length;
            int[] dp = new int[capacity + 1];

            for (int i = 0; i < n; i++) {
                for (int j = weights[i]; j <= capacity; j++) {
                    dp[j] = Math.max(dp[j], dp[j - weights[i]] + values[i]);
                }
            }
            return dp[capacity];
        }

        // 多重背包问题（滚动数组优化）
        public static int multipleKnapsack(int[] weights, int[] values, int[] counts, int capacity) {
            int n = weights.length;
            int[] dp = new int[capacity + 1];

            for (int i = 0; i < n; i++) {
                for (int j = capacity; j >= weights[i]; j--) {
                    for (int k = 1; k <= counts[i] && k * weights[i] <= j; k++) {
                        dp[j] = Math.max(dp[j], dp[j - k * weights[i]] + k * values[i]);
                    }
                }
            }
            return dp[capacity];
        }
    }
}

```

**快读快写**

```java
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;

// 快读类
class FastRead {
    // 创建 StreamTokenizer 对象，用于解析输入流中的标记
    StreamTokenizer st = new StreamTokenizer(new BufferedReader(new InputStreamReader(System.in)));

    // 读取下一个整数的方法
    public int nextInt() throws Exception {
        // 移动到下一个标记
        st.nextToken();
        // 将当前标记的值作为整数返回
        return (int) st.nval;
    }

    public static void main(String[] args) throws IOException {
        //直接在这里创建st对象，来读取int也可以
        StreamTokenizer scan = new StreamTokenizer(new BufferedReader(new InputStreamReader(System.in)));
        scan.nextToken();
        System.out.print((int)scan.nval);
    }
}
```

```java
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

public class FastPrint {
    public static void main(String[] args) {
        PrintWriter pw = new PrintWriter(new OutputStreamWriter(System.out));

        pw.println("Hello World");
        pw.flush();
    }
}
```

