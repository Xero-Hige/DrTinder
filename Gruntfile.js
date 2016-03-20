/*
 * Based on:
 * https://github.com/gruntjs/grunt-contrib-jshint/blob/master/Gruntfile.js
 * https://gist.github.com/lucasp90/e11cf85397394a36e757
 */
'use strict';

module.exports = function(grunt) {
    require('load-grunt-tasks')(grunt);

    // Project configuration.
    grunt.initConfig({
        jshint: {
            all: [
                'Gruntfile.js',
                'tasks/**/*.js',
                '<%= nodeunit.tests %>'
            ],
            options: {
                jshintrc: '.jshintrc'
            }
        },

        jscs: {
            options: {
                config: '.jscsrc',
                reporter: require('jscs-stylish').path,
                esnext: true, // If you use ES6 http://jscs.info/overview.html#esnext
                verbose: true,
                requireCurlyBraces: ['if']
            },
            all: {
                src: ['*/*.js', '*.js']
            }
        },

        // Before generating any new files, remove any previously-created files.
        clean: {
            tests: ['tmp']
        },

        // Configuration to be run (and then tested).
        uglify: {
            compress: {
                files: {
                    'tmp/compress.js': ['test/fixtures/src/simple.js']
                },
                options: {
                    mangle: false
                }
            },
            compressExplicit: {
                files: {
                    'tmp/compress_explicit.js': ['test/fixtures/src/simple.js']
                },
                options: {
                    compress: true
                }
            },
            compressMangle: {
                files: {
                    'tmp/compress_mangle.js': ['test/fixtures/src/simple.js']
                }
            },
            compressMangleBanner: {
                files: {
                    'tmp/compress_mangle_banner.js': ['test/fixtures/src/simple.js']
                },
                options: {
                    banner: '// banner without sourcemap\n'
                }
            },
            noSrc: {
                files: {
                    'tmp/compress_mangle.js': []
                }
            },
            compressMangleExcept: {
                files: {
                    'tmp/compress_mangle_except.js': ['test/fixtures/src/simple.js']
                },
                options: {
                    mangle: {
                        except: ['argumentC']
                    }
                }
            },
            compressMangleBeautify: {
                files: {
                    'tmp/compress_mangle_beautify.js': ['test/fixtures/src/simple.js']
                },
                options: {
                    beautify: true,
                    footer: '\n// This is a footer.'
                }
            },
            enclose: {
                files: {
                    'tmp/enclose.js': ['test/fixtures/src/simple.js']
                },
                options: {
                    beautify: true,
                    compress: false,
                    enclose: {
                        'window.argA': 'paramA',
                        'window.argB': 'paramB'
                    },
                    mangle: false
                }
            },
            multifile: {
                files: {
                    'tmp/multifile.js': ['test/fixtures/src/simple.js', 'test/fixtures/src/comments.js']
                },
                options: {
                    mangle: false
                }
            },
            comments: {
                src: 'test/fixtures/src/comments.js',
                dest: 'tmp/comments.js',
                options: {
                    mangle: false,
                    preserveComments: 'some'
                }
            },
            wrap: {
                src: 'test/fixtures/src/simple.js',
                dest: 'tmp/wrap.js',
                options: {
                    mangle: false,
                    wrap: 'testExport'
                }
            },
            maxLineLen: {
                src: 'test/fixtures/src/simple2.js',
                dest: 'tmp/maxLineLen.js',
                options: {
                    mangle: false,
                    maxLineLen: 100
                }
            },
            ASCIIOnly: {
                src: 'test/fixtures/src/localization.js',
                dest: 'tmp/asciionly.js',
                options: {
                    mangle: false,
                    ASCIIOnly: true
                }
            },
            screwIE8: {
                src: 'test/fixtures/src/screwIE8.js',
                dest: 'tmp/screwIE8.js',
                options: {
                    screwIE8: true
                }
            },
            exportAll: {
                src: 'test/fixtures/src/simple.js',
                dest: 'tmp/exportAll.js',
                options: {
                    mangle: false,
                    wrap: 'testExport',
                    exportAll: true
                }
            },
            sourcemapBasic: {
                src: 'test/fixtures/src/simple.js',
                dest: 'tmp/sourcemap_basic.js',
                options: {
                    sourceMap: true
                }
            },
            sourcemapCustomName: {
                src: 'test/fixtures/src/simple.js',
                dest: 'tmp/sourcemap_customName.js',
                options: {
                    sourceMap: true,
                    sourceMapName: 'tmp/source_map_custom_name'
                }
            },
            sourcemapCustomDir: {
                src: 'test/fixtures/src/simple.js',
                dest: 'tmp/sourcemap_customDir.js',
                options: {
                    sourceMap: true,
                    sourceMapName: 'tmp/deep/directory/location/source_map.js.map'
                }
            },
            sourcemapCustomRoot: {
                src: 'test/fixtures/src/simple.js',
                dest: 'tmp/sourcemap_customRoot.js',
                options: {
                    sourceMap: true,
                    sourceMapRoot: 'https://github.com/RReverser/grunt-contrib-uglify/tree/master/tmp'
                }
            },
            sourcemapCustomUrl: {
                src: 'test/fixtures/src/simple.js',
                dest: 'tmp/sourcemap_customUrl.js',
                options: {
                    sourceMap: true,
                    sourceMapUrl: 'http://www.test.com/test/sourcemap_customUrl.js.map'
                }
            },
            sourcemapFunctionName: {
                src: 'test/fixtures/src/simple.js',
                dest: 'tmp/sourcemap_functionName.js',
                options: {
                    sourceMap: true,
                    sourceMapName: function(dest) {
                        return dest + '.fn.map';
                    }
                }
            },
            sourcemapMultiple: {
                files: {
                    'tmp/sourcemaps_multiple1.js': ['test/fixtures/src/simple.js'],
                    'tmp/sourcemaps_multiple2.js': ['test/fixtures/src/comments.js']
                },
                options: {
                    sourceMap: true
                }
            },
            sourcemapMultipleFunctionNames: {
                files: {
                    'tmp/sourcemaps_multiple1_fnName.js': ['test/fixtures/src/simple.js'],
                    'tmp/sourcemaps_multiple2_fnName.js': ['test/fixtures/src/comments.js']
                },
                options: {
                    sourceMap: true,
                    sourceMapName: function(dest) {
                        return dest + '.fn.map';
                    }
                }
            },
            sourcemapin: {
                files: {
                    'tmp/sourcemapin.js': ['test/fixtures/src/simple2.js']
                },
                options: {
                    mangle: false,
                    banner: '// Hello World\n',
                    sourceMap: true,
                    sourceMapIn: function() {
                        return 'test/fixtures/src/simple2.map';
                    },
                    sourceMapIncludeSources: false
                }
            },
            sourcemapSources: {
                files: {
                    'tmp/sourcemap_sources.js': ['test/fixtures/src/simple.js']
                },
                options: {
                    sourceMap: true,
                    sourceMapIncludeSources: true
                }
            },
            sourcemapinSources: {
                files: {
                    'tmp/sourcemapin_sources.js': ['test/fixtures/src/simple2.js']
                },
                options: {
                    sourceMap: true,
                    sourceMapIn: function() {
                        return 'test/fixtures/src/simple2.map';
                    },
                    sourceMapIncludeSources: true
                }
            },
            expressionJson: {
                files: {
                    'tmp/expression.json': ['test/fixtures/src/simple.json']
                },
                options: {
                    expression: true,
                    mangle: false,
                    compress: false
                }
            },
            expressionJs: {
                files: {
                    'tmp/expression.js': ['test/fixtures/src/expression.js']
                },
                options: {
                    expression: true,
                    mangle: false,
                    compress: false
                }
            },
            mangleprops: {
                files: {
                    'tmp/mangleprops.js': ['test/fixtures/src/mangleprops.js']
                },
                options: {
                    mangleProperties: true
                }
            },
            manglepropsWithExcept: {
                files: {
                    'tmp/mangleprops_withExcept.js': ['test/fixtures/src/mangleprops.js']
                },
                options: {
                    mangle: {
                        except: ['dontMangleMeVariable']
                    },
                    mangleProperties: true
                }
            },
            manglepropsWithExceptionsFiles: {
                files: {
                    'tmp/mangleprops_withExceptionsFiles.js': ['test/fixtures/src/mangleprops.js']
                },
                options: {
                    mangle: {
                        toplevel: true
                    },
                    mangleProperties: true,
                    exceptionsFiles: ['test/fixtures/src/exceptionsfile1.json',
                        'test/fixtures/src/exceptionsfile2.json'
                    ]
                }
            },
            manglepropsWithExceptAndExceptionsFiles: {
                files: {
                    'tmp/mangleprops_withExceptAndExceptionsFiles.js': ['test/fixtures/src/mangleprops.js']
                },
                options: {
                    mangle: {
                        toplevel: true,
                        except: ['dontMangleMeVariable']
                    },
                    mangleProperties: true,
                    exceptionsFiles: ['test/fixtures/src/exceptionsfile1.json',
                        'test/fixtures/src/exceptionsfile2.json'
                    ]
                }
            },
            manglepropsWithNameCacheFile: {
                files: {
                    'tmp/mangleprops_withNameCacheFile1.js': ['test/fixtures/src/mangleprops.js'],
                    'tmp/mangleprops_withNameCacheFile2.js': [
                        'test/fixtures/src/mangleprops_withNameCache.js'
                    ]
                },
                options: {
                    mangle: {
                        toplevel: true
                    },
                    mangleProperties: true,
                    nameCache: 'tmp/uglify_name_cache.json'
                }
            },
            manglepropsWithRegex: {
                files: {
                    'tmp/mangleprops_withRegex.js': ['test/fixtures/src/mangleprops_withRegex.js']
                },
                options: {
                    mangleProperties: {
                        regex: /^[^#].*/
                    }
                }
            },
            quotesSingle: {
                files: {
                    'tmp/quotes_single.js': ['test/fixtures/src/quotes.js']
                },
                options: {
                    quoteStyle: 1
                }
            },
            quotesDouble: {
                files: {
                    'tmp/quotes_double.js': ['test/fixtures/src/quotes.js']
                },
                options: {
                    quoteStyle: 2
                }
            },
            quotesOriginal: {
                files: {
                    'tmp/quotes_original.js': ['test/fixtures/src/quotes.js']
                },
                options: {
                    quoteStyle: 3
                }
            },
            mangleIsNotObject: {
                files: {
                    'tmp/mangle.js': ['test/fixtures/src/simple.js']
                },
                mangle: true
            }
        },

        // Unit tests.
        nodeunit: {
            tests: ['test/*_test.js']
        }
    });

    // task that expects its argument (another task) to fail
    grunt.registerTask('expectFail', function() {
        var task = this.args.join(':');

        var done = this.async();

        function onComplete(error, result) {
            grunt.log.write('\n > ' + result.stdout.split('\n').join('\n > ') + '\n');
            var rv = error ? true : new Error('Task ' + task + ' unexpectedly passed.');
            done(rv);
        }

        grunt.util.spawn({
            grunt: true,
            args: task
        }, onComplete);
    });

    // Actually load this plugin's task(s).
    grunt.loadTasks('tasks');

    // These plugins provide necessary tasks.
    grunt.loadNpmTasks('grunt-contrib-jshint');
    grunt.loadNpmTasks('grunt-jscs');
    grunt.loadNpmTasks('grunt-contrib-clean');
    grunt.loadNpmTasks('grunt-contrib-nodeunit');
    grunt.loadNpmTasks('grunt-contrib-internal');

    // Whenever the 'test' task is run, first clean the 'tmp' dir, then run this
    // plugin's task(s), then test the result.
    grunt.registerTask('test', [
        'jshint',
        'jscs',
        'clean',
        /*   'uglify',*/
        'nodeunit'
    ]);

    // By default, lint and run all tests.
    grunt.registerTask('default', ['test', 'build-contrib']);

};
