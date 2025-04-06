#!/bin/bash
pid=$1
# 获取 RSS 内存值（单位：KB）
rss=$(ps --pid $pid -o rss | tail -n 1)
# 将 KB 转换为 MB
rss_mb=$(bc <<< "scale=2; $rss / 1024")
echo "进程 $pid 实际占用的物理内存大小（MB）：$rss_mb"