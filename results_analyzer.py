#!/usr/bin/python

import sys

class Test:
    def __init__(self, number):
        self.number = number
        self.results = {}
    
    def add_result(self, library, count, time):
        self.results[library] = (count, time)
    
    def to_javascript_string(self):
        output = []
        for i in self.results:
            output.append(str([i, self.results[i][1]]))
        return '[{0}]'.format(','.join(output))
    
    def to_html_table(self):
        template = '''
            <table>
                <thead>
                    <tr>
                        <th>library<th>
                        <th>time taken(seconds)</th>
                        <th>packets per seconds</th>
                    </tr>
                </thead>
                <tbody>
                    {0}
                </tbody>
            </table>
'''
        row_template = '''
                    <tr>
                        <td>{0}</td>
                        <td>{1}</td>
                        <td>{2}</td>
                    </tr>'''
        output = ''
        for i in self.results:
            time_taken = self.results[i][1] / 1000.0
            output += row_template.format(
                i, 
                time_taken, 
                int(self.results[i][0] / time_taken)
            )
        return template.format(output)

class Analyzer:
    html_graphic_template_file = 'html-graphics-template.html'
    
    def __init__(self, results_file):
        fd = open(results_file)
        self.tests = {}
        for line in fd:
            line = line.strip()
            data = line.split(' ')
            if len(data) != 4:
                raise Exception("Invalid file")
            if data[0] not in self.tests:
                self.tests[data[0]] = Test(data[0])
            self.tests[data[0]].add_result(data[1], int(data[2]), int(data[3]))
    
    def export_to_html_graphic(self, output_stream):
        template = open(Analyzer.html_graphic_template_file).read()
        for test in self.tests:
            tag = '{{TEST{0}_DATA}}'.format(test)
            template = template.replace(tag, self.tests[test].to_javascript_string())
        output_stream.write(template)
        output_stream.close()
    
    def export_to_html_table(self, output_stream):
        for test in self.tests:
            data = self.tests[test].to_html_table()
            output_stream.write('<h1>Test{0}</h1>\n'.format(test))
            output_stream.write(data)
        output_stream.close()


if len(sys.argv) < 3:
    print 'Usage ' + sys.argv[0] + ' <input_file> <output_type> [output_file]'
    print ''
    print 'output_type can be:'
    print '\thtml-graphic'
    print '\thtml-table'
    print '\tlatex-table'
    exit(1)


if len(sys.argv) == 4:
    output = open(sys.argv[3], 'w')
else:
    output = sys.stdout
    
analyzer = Analyzer(sys.argv[1])
if sys.argv[2] == 'html-graphic':
    analyzer.export_to_html_graphic(output)
elif sys.argv[2] == 'html-table':
    analyzer.export_to_html_table(output)
elif sys.argv[2] == 'latex-table':
    analyzer.export_to_latex_table(output)
    
