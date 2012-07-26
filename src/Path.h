#ifndef Path_h
#define Path_h

#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <Set.h>
#include <ByteArray.h>

class Path : public ByteArray
{
public:
    Path(const Path &other)
        : ByteArray(other)
    {}
    Path(const ByteArray &other)
        : ByteArray(other)
    {}
    Path(const char *path)
        : ByteArray(path)
    {}
    Path(const char *path, int size)
        : ByteArray(path, size)
    {}
    Path() {}
    Path &operator=(const Path &other)
    {
        ByteArray::operator=(other);
        return *this;
    }
    Path &operator=(const ByteArray &other)
    {
        ByteArray::operator=(other);
        return *this;
    }

    Path &operator=(const char *path)
    {
        ByteArray::operator=(path);
        return *this;
    }

    bool operator==(const Path &other) const
    {
        return Path::resolved(*this).ByteArray::operator==(Path::resolved(other));
    }

    bool operator!=(const Path &other) const
    {
        return !Path::operator==(other);
    }

    enum Type {
        Invalid,
        File,
        Directory,
        CharacterDevice,
        BlockDevice,
        NamedPipe,
        SymLink,
        Socket
    };

    inline bool exists() const { return type() != Invalid; }
    inline bool isDir() const { return type() == Directory; }
    inline bool isFile() const { return type() == File; }
    inline bool isSocket() const { return type() == Socket; }
    inline bool isAbsolute() const { return (!isEmpty() && at(0) == '/'); }
    inline bool isSymLink() const { return type() == SymLink; }
    Path followLink(bool *ok = 0) const;
    const char *fileName() const;
    const char *extension() const;
    static bool exists(const Path &path) { return path.exists(); }
    static bool mkdir(const Path &path);
    static bool rm(const Path &file);
    static Path home() { return Path::resolved(getenv("HOME")); }
    bool mksubdir(const ByteArray &subdir) const;
    bool isSource() const;
    static bool isSource(const char *extension, int len);
    bool isSystem() const { return Path::isSystem(constData()); }
    static bool isSystem(const char *path);
    bool isHeader() const;
    Path parentDir() const;
    Type type() const;
    bool resolve(const Path &cwd = Path());
    int canonicalize();
    time_t lastModified() const; // returns time_t ... no shit
    int64_t fileSize() const;
    static Path resolved(const ByteArray &path, const Path &cwd = Path(), bool *ok = 0);
    static Path canonicalized(const ByteArray &path);

    enum VisitResult {
        Abort,
        Continue,
        Recurse
    };
    typedef VisitResult (*VisitCallback)(const Path &path, void *userData);
    void visit(VisitCallback callback, void *userData) const;
};

#endif
