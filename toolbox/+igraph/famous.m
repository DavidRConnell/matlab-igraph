function graph = famous(name, graphOpts)
%FAMOUS returns a famous graph.
%   GRAPH = FAMOUS(NAME) Get the GRAPH identified by NAME.
%
%   Available graphs are:
%
%       'bull', 'chvatal', 'coxeter', 'cubical', 'diamond', 'dodecahedral',
%       'dodecahedron', 'folkman', 'franklin', 'frucht', 'grotzsch', 'heawood',
%       'herschel', 'house', 'housex', 'icosahedral', 'icosahedron',
%       'krackhardt_kite', 'levi', 'mcgee', 'meredith', 'noperfectmatching',
%       'nonline', 'octahedral', 'octahedron', 'petersen', 'robertson',
%       'smallestcyclicgroup', 'tetrahedral', 'tetrahedron', 'thomassen',
%       'tutte', 'uniquely3colorable', 'walther', 'zachary'.
%
%   GRAPH = GENERATE(..., 'PARAM1', VAL1, 'PARAM2', VAL2, ...) there are
%   parameters REPR and DTYPE for specifying how the resulting GRAPH is
%   represented common to all methods. See the "functions returning graphs"
%   section in help IGRAPH for more information.
%
%   See also IGRAPH.GENERATE, IGRAPH.GAME.

    arguments
        name (1, :) char ...
            {igutils.mustBeMemberi(name, ...
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
                                    'zachary'})};
        graphOpts.?igutils.GraphOutProps;
    end

    graphOpts = namedargs2cell(graphOpts);
    graphOpts = igutils.setGraphOutProps(graphOpts{:});

    graph = mexIgraphDispatcher(mfilename(), lower(name), graphOpts);
end
