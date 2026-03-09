# project5

Console C application with SQLite for variant 13 (`Person`).

## Features
- Menu-based console interface.
- SQLite connection and SQL operations in code.
- Parameterized queries (`SELECT`, `INSERT`, `DELETE`).
- Filters:
  - by `id`
  - by `last_name` pattern (`LIKE`)
  - by common field `city`
- Inserts in autocommit mode and transaction mode.
- BLOB photo attach/export.

## Structure
- `include/` - headers (model/service/interface contracts).
- `src/` - implementation.
- `data/` - SQLite DB and binary files.
- `Makefile` - build and run.

## Build
```bash
cd project5
make
```

## Run
```bash
cd project5
make run
```

## Recommended demo flow
1. Menu `1` - init schema.
2. Menu `2` - insert 1 person (autocommit).
3. Menu `3` - insert 3 persons (transaction).
4. Menu `4/5/6/7` - select and filters.
5. Menu `9` - attach `data/sample_photo.bin` to any inserted `id`.
6. Menu `10` - export photo to `data/exported_photo.bin`.
