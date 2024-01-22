# ruler

Ruler is a system for interacting with design rules for VLSI design. While this
could turn into a DRC checker in the long term, the short term goals are for
this to be used for automated cell layout.

## Build and Install


## Examples

## Notes

When comparing two boolean expressions, there is a concept of minterms. A
minterm completely describes a single situation on which the boolean expression
can be applied. In geometry, the equivalent concept would be a collection of
polygons representing a single situation on which those rules can be applied.
So, create a set of geometry "primitives" to operate as the basis for defining
minterms of geometric operations. Execute the geometric operations on those
primitives, then compare the results. This will allow you to compare two
expressions on those primitives even if the two expressions are implemented
differently. So, what are the "primitives" we'd need for the operators we have,
and how do we make those primitives robust to transformations that involve real
units? This would allow us to compare subset/not subset.

Beyond that, how do we create a way to search for a rule? With boolean
expressions, we expanded the expression into it's minterms and then did a
search on those minterms. Somehow we need a way to create an internally
consistent less than/greater than operator?

Or, hash map of minterms that point back to a rule.

So the rule lookup works as follows:
1. evaluate rule terms on all primitives
2. associate each primitive with a hashmap of outcomes representing "minterms"
3. the key of the hashmap is the minterm, and the value is a pointer to the rule.
4. if both sides of the rule are satisfied by a minterm of the user's input, then return the rule's value and name.

So, in order for this to work, I need to first create a drc engine.

DRC Engine -> DRC Rule Lookup -> Cell Layout Engine
         |--> Real time iterative DRC

is it possible to define this only for advanced nodes using only rectangles? Or do I need to implement for arbitrary polygons?

