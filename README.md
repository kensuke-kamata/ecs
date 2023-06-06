# ECS

```
| Entities (index) | Component 0   | Component 1   | Componen 2    |
|------------------|---------------|---------------|---------------|
| Entity (0)       |      1        |      0        |     0         | <- bitset
| Entity (1)       |      0        |      1        |     1         | <- bitset
| Entity (2)       |      1        |      0        |     1         | <- bitset
|------------------|---------------|---------------|---------------|
|                  |[ComponentPool]|[ComponentPool]|[ComponentPool]|

- Entity[]        = [(index 0), (index 1), (index 2), ... , (index N)]
- ComponentPool[] = [sizeof(Component X) x Entity N]
```
