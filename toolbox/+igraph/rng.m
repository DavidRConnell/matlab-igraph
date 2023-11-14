function rng(seed, generator)
%RNG modify the random number generator used by igraph.
%   RNG(SD) seeds the rng with SD. The seed must be a non-negative integer.
%   RNG(SD, GENERATOR) sets the generator used by igraph's default rng to
%      GENERATOR. GENERATOR can be one of:
%
%       Generator    Description
%      ----------------------------------------------------------------------
%       'MT19937'     Makoto Matsumoto and Takuji Nishimura variant of
%                     Merseene Twister (default)
%       'glibc2'      GNU libc 2
%       'pcg32'       32-bit PCG random number generator
%       'pcg64'       64-bit PCG random number generator
%
%   NOTE: This does not impact Matlab's random number generator. If
%   reproducible behavior is desired and random number from both Matlab and
%   igraph are needed, set both generators.
%
%   Both Matlab and igraph default to MT19937 so using the same seed for both
%   will lead to repeated values.

    arguments
        seed {mustBeNonnegative,mustBeInteger}
        generator (1, :) char {mustBeMemberi(generator, ...
                                             {'mt19937', 'glibc2', 'pcg32', ...
                                              'pcg64', 'current'})} = "Current";
    end

    mexIgraphRNG(seed, lower(generator));
end
