function score = compare(comm1, comm2, method)
%COMPARE score the similarity of two membership vectors
%   SCORE = COMPARE(COMM1, COMM2) return a score of how similar the membership
%   vector COMM1 is to membership vector COMM2.
%
%   SCORE = COMPARE(COMM1, COMM2, METHOD) use method METHOD to compare the
%   membership vectors. Valid methods are:
%
%       Name                        Method
%   -------------------------------------------------------------------------
%      'nmi'                    normalized mutual information (the default)
%      'vi'                     variation of information
%      'splitjoin'              split-join distance
%      'rand'                   rand index
%      {'ari','adjustedrand'}   adjusted rand index
%
%   See also IGRAPH.CLUSTER.

    arguments
        comm1 {igutils.mustBePartition}
        comm2 {igutils.mustBePartition, mustBeEqualLength(comm1,comm2)}
        method (1, :) char ...
            {igutils.mustBeMemberi(method, {'vi','nmi','splitjoin','rand',...
                                            'adjustedrand','ari'})} = 'nmi';
    end

    method = lower(method);

    if strcmp(method, 'splitjoin')
        method = 'split_join';
    end

    if strcmp(method, 'adjustedrand') || strcmp(method, 'ari')
        method = 'adjusted_rand';
    end

    score = mexIgraphDispatcher(mfilename(), comm1, comm2, method);
end

function mustBeEqualLength(a, b)
    if ~isequal(length(a), length(b))
        eid = 'igraph:notEqualLength';
        msg = 'Length of first membership vector must be the same as second membership vector.';
        throwAsCaller(MException(eid, msg));
    end
end
