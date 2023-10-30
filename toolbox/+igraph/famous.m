function adj = famous(name, adjOptions)
%FAMOUS returns a famous graph.
%   ADJ = FAMOUS(NAME) Get the named graph.
%       Available graphs are:
%
%       'bull', 'chvatal', 'coxeter', 'cubical', 'diamond', 'dodecahedral',
%       'dodecahedron', 'folkman', 'franklin', 'frucht', 'grotzsch', 'heawood',
%       'herschel', 'house', 'housex', 'icosahedral', 'icosahedron',
%       'krackhardt_kite', 'levi', 'mcgee', 'meredith', 'noperfectmatching',
%       'nonline', 'octahedral', 'octahedron', 'petersen', 'robertson',
%       'smallestcyclicgroup', 'tetrahedral', 'tetrahedron', 'thomassen',
%       'tutte', 'uniquely3colorable', 'walther', 'zachary'.
%
%   ADJ = GENERATE(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) there are
%       parameters for specifying the how the resulting ADJ is represented.
%       These follow the same conventions as the rest of matlab-igraph.
%
%        Name           Description
%       --------------------------------------------------------------------
%        'makeSparse'   Whether to return a sparse (default) or full matrix.
%        'dtype'        The data type to use, either 'double' (default) or
%                       'logical'.
%
%   See also igraph.generate, igraph.game.

    arguments
        name (1, :) char ...
            {mustBeMemberi(name, ...
                           {'bull', 'chvatal', 'coxeter', 'cubical', ...
                            'diamond', 'dodecahedral', 'dodecahedron', ...
                            'folkman', 'franklin', 'frucht', 'grotzsch', ...
                            'heawood', 'herschel', 'house', 'housex', ...
                            'icosahedral', 'icosahedron', ...
                            'krackhardt_kite',  'levi', 'mcgee', ...
                            'meredith', 'noperfectmatching', 'nonline', ...
                            'octahedral', 'octahedron', 'petersen', ...
                            'robertson', 'smallestcyclicgroup', ...
                            'tetrahedral', 'tetrahedron', 'thomassen', ...
                            'tutte', 'uniquely3colorable', 'walther', ...
                            'zachary'})}
        adjOptions.makeSparse (1, 1) logical = true;
        adjOptions.dtype (1, :) char {mustBeMemberi(adjOptions.dtype, ...
                                                    {'logical', 'double'})} = ...
                                                    'double';
    end

    adj = mexIgraphFamous(lower(name), adjOptions.makeSparse, ...
                          adjOptions.dtype);
end
