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

    arguments
        seed {mustBeNonnegative,mustBeInteger}
        generator (1, :) char {mustBeMemberi(generator, ...
                                             {'mt19937', 'glibc2', 'pcg32', ...
                                              'pcg64', 'current'})} = "Current";
    end

    mexIgraphRNG(seed, lower(generator));
end
