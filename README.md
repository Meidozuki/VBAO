# VBAO

> A lite library aiming for building an MVVM (or MVFM) project.

## Install and start

### Python

```shell
conda activate YOURENV
pip install vbao-mvvm
```

```python
import vbao
```

## How does it work?

We implement the data-binding, command-binding and property-change-notification to form an MVVM system.

![](img/binding.dot.jpg)

data-binding & command-binding

![](img/changing.dot.jpg)

property-change-notification

When we make it, the interaction among them is as follows.

![](img/action.dot.jpg)

### Filetree

├── example  
│   ├── app  
│   ├── model  
│   ├── view  
│   ├── viewmodel  
│   └── window  
├── extern  
│   └── pybind11  
├── Lib_VBao  
│   ├── python  
│   │   ├── vbao  
│   └── VBAO  
├── LICENSE  
└── README.md


### Special Thanks

[Zhejiang-University-GKC/SJDXQcourseware: The courseware of SJDXQ (github.com)](https://github.com/Zhejiang-University-GKC/SJDXQcourseware)
