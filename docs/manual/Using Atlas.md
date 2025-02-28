# Using Projects {#using-projects}

---

Atlas interacts with your game through the IProject class. More specifically, it interacts through the derived class. Within `project.h` in the `project_lib` project, you will see this at the top of the file:

```
#ifndef ATLAS_BUILD_EXAMPLES
	#define PROJECT_TYPENAME Project
#else
	...
#endif
```

Now what does this mean? This is how Atlas knows what your derivative class is. If you are using the already provided class, then this does not have to be worried about. However, this is used when you need to have a lower-level control of the engine. Atlas interacts through the various virtual functions that are found within IProject in \ref Project.h. If you have questions about these events, please refer to IProject.