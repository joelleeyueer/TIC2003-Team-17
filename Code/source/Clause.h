#pragma once

enum ClauseType {
    CLAUSE_SUCH_THAT,
    CLAUSE_PATTERN
};

struct Clause {
    Clause() {}
    virtual ~Clause() = default;
    virtual ClauseType getType() = 0;
};
