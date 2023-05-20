#ifndef SEMANTIC_H
#define SEMANTIC_H

/**
 * It contains the flags of a variable descriptor.
 */
enum semantic_flags_t { IS_CONSTANT = 1 };

/**
 * A variable descriptor that contains the semantic
 * information about a variable.
 */
struct var_descriptor_t {
  double value;
  enum semantic_flags_t flags;
};

#endif // SEMANTIC_H
