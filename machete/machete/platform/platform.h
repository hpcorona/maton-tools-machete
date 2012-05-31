//
//  platform.h
//  machete
//
//  Created by Hilario Perez Corona on 6/15/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file platform.h
//! \brief Platform specific helper classes and data structures.

#pragma once

#include "../math/vector.h"
#include "../common/log.h"

namespace machete {
  
  //! Platform interface.
  /*!
   Must implement one class for each platform.
   */
  class IPlatform {
  public:
    
    //! Get the full resource path.
    /*!
     All files available for the application must be contained here.
     
     Only one resource path is supported.
     
     \return A character string (null terminated) containing the resource path.
     */
    virtual const char* GetResourcePath() const = 0;
    
    //! Load an image form the resource path.
    /*!
     This method will NOT free up any resources.
     
     \param filename The name of the file to load.
     \param data An address to the data pointer. The implementation will create a new data pointer.
     \param size The size of the loaded image.
     */
    virtual void LoadImage(const char* filename, void **data, machete::math::IVec2 & size) = 0;
    
    //! Unload the last image and free up resources.
    virtual void UnloadImage() = 0;
    
    //! Load a file form the resources path.
    /*!
     The entire file will be loaded into memory.
     
     The called will need to release the memory requested.
     
     \param name File name to load.
     \param data An address to the data pointer. The entire file will be loaded into memory.
     \return The number of bytes that was loaded into memory. No additional bytes are added.
     */
    virtual unsigned int LoadFile(const char* name, char** data) = 0;
    
    //! Load an audio file from the resources path.
    /*!
     \param name File name to load.
     \return The OpenAL buffer id that was created.
     */
    virtual unsigned int LoadAudio(const char* name) = 0;

    //! Generates a random integer.
    /*!
     \return A random integer.
     */
    virtual unsigned int Random() = 0;
    
    //! Open a file.
    /*!
     \param name File name.
     \param size The file size. Zero if it's a direct file (not an archived one).
     \return The file descriptor.
     */
    virtual FILE* OpenFile(const char* name, unsigned long &size) = 0;
    
    //! Close a file.
    /*!
     \param hande File handle.
     */
    virtual void CloseFile(FILE* handle) = 0;
    
    //! Get the name of a complete writable file path.
    virtual char* WritableFile(const char* name) = 0;
    
    //! Get the absolute system time.
    virtual double AbsoluteTime() = 0;
    
    //! Launch a website.
    virtual void LaunchURL(const char* url) = 0;

		//! Exit the application.
		virtual void Exit() = 0;
  };

  //! Platform specific features.
  namespace platform {
        
    //! Global Platform manager.
    extern IPlatform* ThePlatform;

  }
}
