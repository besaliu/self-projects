def find_paths(connection: list[list[int]], source: int, destination: int) -> list[list[int]]:
    # First, construct an adjacency list from the connection matrix
    adj_list = {i: [] for i in range(len(connection))}
    for u, v in connection:
        adj_list[u].append(v)

    # Perform a depth-first search from the source node to the destination node,
    # keeping track of the path taken so far
    def dfs(node, path):
        # If we've reached the destination node, add the current path to the result
        if node == destination:
            result.append(path[:])
            return

        # Recursively explore all neighbors of the current node
        for neighbor in adj_list[node]:
            path.append(neighbor)
            dfs(neighbor, path)
            path.pop()

    # Initialize the result list and start the DFS from the source node
    result = []
    dfs(source, [source])

    return result

