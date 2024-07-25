classdef TestMxRandom < matlab.unittest.TestCase
    properties
        low = 1;
        high = 10;
    end

    properties (TestParameter)
        seed = {1, 123, 3949, 223};
    end

    methods (Test)
        function testProduceSameIntegers(testCase, seed)
            l = testCase.low;
            h = testCase.high;

            rng(seed);
            expected1 = randi([l h], 1);
            actual2 = igraphRandi(l, h);

            rng(seed);
            actual1 = igraphRandi(l, h);
            expected2 = randi([l h], 1);

            testCase.verifyEqual(actual1, expected1)
            testCase.verifyEqual(actual2, expected2)
        end
    end
end
