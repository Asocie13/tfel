The `@Times` keywords let the user specify a list of times used for
the computations.

This keywords is followed by an array describing intervals of
times. By default, one time step is used to go from a given time to
the next one. The keyword 'in' allows the user to divide an interval
into a given number of smaller intervals.

## Note

The loadings and the external state variables evolutions are
described in a totally independent manner (see the `@ImposedStrain`,
the `@ImposedStress` and the `@ExternalStateVariable` keywords).

## Note

In case of non convergence, the algorithm handles time sub
steppings. See the `@MaximumNumberOfSubSteps` for details.

## Example

~~~~ {.cpp}
@Times {0.,3600.};         // 1  time step of  3600. seconds
~~~~~~~~

## Exemple

~~~~ {.cpp}
@Times {0.,3600. in 10};   // 10 time steps of 360   seconds
~~~~~~~~