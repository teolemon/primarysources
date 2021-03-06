// Copyright 2015 Google Inc. All Rights Reserved.
// Author: Sebastian Schaffert <schaffert@google.com>

#include "Statement.h"

bool operator==(const Time& lhs, const Time& rhs) {
    return lhs.year == rhs.year &&
           lhs.month == rhs.month &&
           lhs.day == rhs.day &&
           lhs.hour == rhs.hour &&
           lhs.minute == rhs.minute &&
           lhs.second == rhs.second;
}

ApprovalState stateFromString(const std::string &state) {
    if (state == "approved") {
        return APPROVED;
    } else if(state == "wrong") {
        return WRONG;
    } else if(state == "skipped") {
        return SKIPPED;
    } else if(state == "othersource") {
        return OTHERSOURCE;
    } else if(state == "unapproved") {
        return UNAPPROVED;
    } else if(state == "duplicate") {
        return DUPLICATE;
    } else if(state == "blacklisted") {
        return BLACKLISTED;
    } else {
        throw InvalidApprovalState("Bad Request: invalid or missing state parameter ("+state+")");
    }
}

std::string stateToString(ApprovalState state) {
    return "";
}

void Value::serialize(cppcms::archive &a) {
    a & str & lang & cppcms::as_pod(time) & loc & precision
      & cppcms::as_pod(quantity) & cppcms::as_pod(type);
}

void PropertyValue::serialize(cppcms::archive &a) {
    a & property & value;
}

void Statement::serialize(cppcms::archive &a) {
    a & id & qid & propertyValue & qualifiers & sources
      & cppcms::as_pod(approved);
}

bool operator==(const Value& lhs, const Value& rhs) {
    if (lhs.type != rhs.type) return false;
    switch (lhs.type) {
        case ITEM:
            return lhs.str == rhs.str;
        case STRING:
            return lhs.str == rhs.str && lhs.lang == rhs.lang;
        case TIME:
            return lhs.time == rhs.time
                   && lhs.precision == rhs.precision;
        case QUANTITY:
            return lhs.quantity == rhs.quantity;
        case LOCATION:
            return lhs.loc == rhs.loc;
    }
    return false;
}

bool operator==(const PropertyValue& lhs, const PropertyValue& rhs) {
    return lhs.property == rhs.property && lhs.value == rhs.value;
}

bool operator==(const Statement& lhs, const Statement& rhs) {
    if (lhs.qid != rhs.qid) return false;
    if (lhs.propertyValue != rhs.propertyValue) return false;
    if (lhs.qualifiers.size() != rhs.qualifiers.size()) return false;
    if (lhs.sources.size() != rhs.sources.size()) return false;
    for (auto x : lhs.qualifiers) {
        bool found = false;
        for (auto y : rhs.qualifiers) {
            if (x == y) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    for (auto x : lhs.sources) {
        bool found = false;
        for (auto y : rhs.sources) {
            if (x == y) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    return true;
}
