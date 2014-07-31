#include <stdexcept>
#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <boost/filesystem.hpp>
#include <SFML/Graphics.hpp>


/* Returns a list of path to all images contained in dir
 *
 */
std::list<std::string> getImagesFromDir(const std::string& dir)
{
    // Check dir
    if(!boost::filesystem::exists(dir))
        throw std::runtime_error(dir + " does not exist");
    if(!boost::filesystem::is_directory(dir))
        throw std::runtime_error(dir + " is not a directory");

    std::list<std::string> names;
    
    for(auto it = boost::filesystem::directory_iterator(dir);
        it != boost::filesystem::directory_iterator();
        ++it)
    {
        boost::filesystem::path path = it->path();
        if(path.has_extension() && path.extension().compare(".bmp") == 0)
            names.push_back(path.string());
    }

    return names;
}

/* Compute size of sprite sheet based on the first image. We assume that all 
 * images have the same size. 
 */
void computeSize(sf::Image& output, const std::string& firstPath, const std::size_t nbImages)
{
    sf::Image i;
    if(!i.loadFromFile(firstPath))
        throw std::runtime_error(std::string("could not load image from path ") + firstPath);

    unsigned int w = i.getSize().x;
    unsigned int h = i .getSize().y;

    unsigned int nbColumns = static_cast<unsigned int>(sqrt(static_cast<double>(nbImages)));
    unsigned int nbRows = static_cast<unsigned int>(nbImages) / nbColumns;
    if(nbRows * nbColumns < nbImages)
        ++nbRows;

    output.create(w * nbColumns, h * nbRows);
}

/* Fill sprite sheet from list of images.
 */
void fillImage(sf::Image& output, const std::list<std::string>& images)
{
    unsigned int x = 0, y = 0;

    for(auto it = images.begin();
        it != images.end();
        ++it)
    {
        sf::Image i;
        if(!i.loadFromFile(*it))
            throw std::runtime_error(std::string("could not load image from path ") + *it);

        unsigned int width = i.getSize().x;
        unsigned int height = i .getSize().y;
        output.copy(i, width * x, height * y);

        ++x;
        if(x * width == output.getSize().x)
        {
            x = 0;
            ++y;    
        }
    }
}

/*  Create a sprite sheet
 *  dir: Path to a directory containing a list of images
 *  outputPath: Full path where the sprite sheet will be saved
 */
void createSpriteSheet(const std::string& dir, const std::string& outputPath)
{
    // Get list of images
    std::list<std::string> images = getImagesFromDir(dir);
    if(images.empty())
        throw std::runtime_error(std::string("no png images found in ") + dir);

    sf::Image output;
    computeSize(output, images.front(), images.size());
    fillImage(output, images);

    // Save output image
    if(!output.saveToFile(outputPath))
        throw std::runtime_error(std::string("could not save image to ") + outputPath); 
}

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        std::cerr << "Not enough arguments given." << std::endl;
        std::cerr << "./spritesheetmaker <dir> <output_image>" << std::endl;
        return -1;
    }
    
    try
    {
        std::string dir = argv[1];
        std::string outputPath = argv[2];
        createSpriteSheet(dir, outputPath);
    }
    catch(std::exception &e)
    {
        std::cerr << "Error while creating spritesheet. Reason: " << e.what() << '.' << std::endl;
        return -1;
    }
    
    return 0;
}

