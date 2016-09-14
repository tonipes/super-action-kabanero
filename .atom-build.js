module.exports = {
  cmd: "cd build && cmake ..",
  preBuild: function () {
    console.log('This is run **before** the build command');
  },
  postBuild: function () {
    console.log('This is run **after** the build command');
  },
  targets: {
    build: {
      cmd: "cd build && cmake .."
    },
    "build and run": {
      cmd: "cd build && cmake ../ && make && ./sfml-base"
    }
  }
};
