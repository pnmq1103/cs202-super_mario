# Coding
## Class names & Member functions
Use `CamelCase` starting with a capital letter.
```cpp
class MyClass {
 public:
  void DoSomething();
  int GetCount() const;
};
```

## Private data members
Use `snake_case_`, ending with an underscore.
```cpp
class MyClass {
 private:
  int count_;
  std::string name_;
};
```

## Public, Protected data members and variables
Use `snake_case`.
```cpp
class MyClass {
 protected:
  int max_count;
};

void DoSth() {
    Vector2 world_position = {0,0};
}
```

## Static or Constant members
Use `camelCase`.
```cpp
class MyClass {
 private:
  static const int maxCount = 100;
};
```

# Files and Folders 
Below are the main folders
├───docs
├───res
│   ├───music
│   └───sprites
├───src

The purpose of each folder:
- docs: contains latex, markdown file to document to project.
- res: contains project resources such as sprites, music, image, etc.
- src: contains source files and header files.

The main folders can include other sub-folders.

## Naming 
- The name should be meaningful, descriptive and concise.
- Use `camel_case` for naming, e.g. `character_state.cpp`, `background_music.mp3`.
