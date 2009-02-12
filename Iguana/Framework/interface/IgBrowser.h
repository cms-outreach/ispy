#ifndef IGUANA_FRAMEWORK_IG_BROWSER_H
# define IGUANA_FRAMEWORK_IG_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepresentable;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/**
@page architecturalDesign IGUANA Browser Architecture
@section overview Overview

  The main units relating to the IGUANA architecture are:

  - A thin <i>portability</i> and <i>utilities</i> layer;
  - A small <i>kernel</i> that manages a number of <i>plug-ins</i>:
    - <i>application personalities</i>;
    - a <i>session</i> with <i>extensions</i> forming the <i>shared
      application state</i>;
    - user interface components: <i>sites</i> and <i>browsers</i>;
    - <i>representation methods</i> to map between the experiment
      objects and the various browsers;
  - <i>External software imported into IGUANA</i> for convenience of
    building and distribution, and <i>external software which remains
    outside IGUANA.</i>

  The first of these, the protability layer, is mostly implemented by
  the @c classlib package, an independent library providing extensive
  debugging facilities, many useful little classes and
  platform-independent interfaces to sockets, file system,
  high-resolution timers, shared libraries and more.

  The second, the kernel, is Implemented by @c IgPluginManager, a
  generic plug-in manager package, which is extended by the
  architecture kernel package @c IgObjectBrowser that provides the
  core functionality (session objects, extensions, user interface
  components, representations, ...)  and base interfaces for the
  various plug-in types.

  The plug-ins are in the @c Ig_Modules subsystem which includes the
  event display core, application management services, and thin
  adaptors to export external software in forms understood by the
  architecture.  In short, all the real functionality.  Experiments
  build plug-ins similar to these.  The kernel loads the modules
  dynamically as required.  (The plug-ins are normally demand-loaded
  shared libraries but we also support statically linked applications;
  no shared libraries need to be involved.)

  The external software is the largest chunk of all, including the 3D
  graphics (OpenGL, OpenInventor) and GUI kernels (Qt), a XML parser,
  various packages for plotting and interactive analysis,
  public-domain Qt and Inventor extensions and so forth.  It also
  includes all experiment software.

@section arch Architecture

  How does one provide a <i>generic</i> tool that can integrate with a
  <i>specific</i> experiment or even a single task within it?  There
  seems to be convergence towards two architecture models to address
  the issues.  The first approach is to standardise on a common object
  description.  Thus the analysis environment can understand all
  objects and can provide complete access to data and algorithms.  The
  description may be common base classes or sufficiently rich
  introspection.  The <a href="http://root.cern.ch/">ROOT</a> project
  is perhaps the most significant proponent of this approach and has
  built a very extensive environment around the concept.  The other
  approach is to use abstract interfaces to express the areas where
  interaction with external packages is required; the experiments
  provide their side of the interfaces.  Generic analysis tools
  implement several useful interfaces to allow a physicist to use the
  product ``out of the box,'' while at the same time allowing for
  extensibility.  Several projects such as <a
  href="http://www-sldnt.slac.stanford.edu/jas/">JAS</a>, <a
  href="http://cern.ch/anaphe/">Lizard</a> and <a
  href="http://http://www.lal.in2p3.fr/OpenScientist/">OpenScientist</a>
  seem to be following this path.

  IGUANA follows roughly the latter direction.  It has a small kernel,
  everything else is implemented in demand-loaded <i>extensions</i>.
  Our intention is for the toolkit to be extended to create
  experiment-specific programs---it is not an end-user product that
  can be used as is.  In terms of design we do not specify a fixed
  list of abstract interfaces, there are just ``extensions.''  Each
  component negotiates the specific services it needs.  This allows
  new services to be added quickly, to be propagated into existing
  code gradually, and existing services to be altered to do new things
  easily.  Conceptually there are some similarities with <a
  href="http://cobra.web.cern.ch/cobra/">COBRA</a> and <a
  href="http://lhcb-comp.web.cern.ch/lhcb-comp/Frameworks/Gaudi/">Gaudi</a>.

 @subsection archapp Application Personalities

  When an IGUANA program runs, it first creates a <i>session</i>
  object into which it then attaches an <i>application
  personality:</i> the main program that determines all subsequent
  behaviour (#IgSession, #IgDriver).  Typically the personality
  immediately extends the session object with <i>services</i>
  pertinent to the purpose of the application (#IgExtension).  For
  example, a graphical personality would create a main window and add
  services that give access to the menus and maintain GUI event loops.
  The personality then loads a number <i>extensions</i> into the
  session; a graphical personality would tell the system to load all
  ``GUI extensions'' (#IgExtension).  Hence it is the personality and
  the extensions that together form the application: the personality
  exposes features by installing service interfaces into the session,
  based which other extensions can provide further services and make
  features available to the user.  IGUANA's flexibility comes largely
  from the combination of extensions and the session as the <i>shared
  application state:</i> different parts of the application can
  collaborate and new features can be deployed into all application
  personalities in one go with an auto-loaded extension.

 @subsection archgui Browsers and Sites

  An interactive application personality then creates its user
  interface.  An important part of our design are user interfaces that
  are modular but seamless and which can be created dynamically, for
  example from a textual description.  We reify two user interface
  components: <i>browsers</i> and <i>sites</i> (#IgBrowser, #IgSite).
  A browser is a way to look at and interact with application
  objects---for example a 3D event display or a property sheet.  It
  may also control other things, such as a cut in a histogram display.
  A browser does not however have to graphical: it might just sit on
  the background and respond to other browsers' queries such as ``What
  actions can be invoked on this object?''  Sites host browsers, by
  providing for example a window for a 3D browser view.  More
  generally sites expose objects such as GUI widgets as hosts for
  other sites and browsers.  Sites need not be graphical either: a
  pipe site might host a browser communicating to a subprocess.

 @subsection archrep Object Representations

  Application objects can be represented in many different browsers.
  A browser typically has a <i>model</i> made of a number of object
  <i>reps</i> (#IgModel, #IgRep).  We encourage the use of
  model-specific reps, such as 3D shape objects, not the application
  objects themselves.  This permits an object's representation to be
  separated from the object itself---and to be varied by the
  situation.  It also clarifies object design substantially and helps
  to control software dependencies.  As we associate all reps of an
  object together and to the object, the separation does not result in
  any loss of functionality.  It is possible to correlate the object's
  reps in different browsers and to create a rep in a right context in
  each browser.  A browser can also interact directly with the
  application objects if it wants to.

  Our implementation requires ``representable'' application objects to
  inherit from a common base type.  One can either use our type
  (#IgRepresentable) or make our type an alias for another class.  The
  only requirement is that the type must be polymorphic; ours simply
  defines a virtual destructor and no other methods.  In CMS there is
  however no base type shared by all objects; for visualisation we use
  proxies that inherit #IgRepresentable and point to other CMS
  objects.  The primary reason for this choice is our framework's
  support for ``virtual objects'' that can be materialised upon
  request; we must of course present the user an option to display
  such objects whether or not they have been created.  We also want to
  keep the application object design separate from their
  representation as discussed above.  Moreover we need several views
  of the objects, an issue which we solve in part with multiple
  proxies.

  The object--rep mappings are extensions loaded dynamically on
  demand.  IGUANA discovers and chooses the right mapping
  automatically; the extension code simply does whatever is
  appropriate for that combination.  It is not necessary to have a
  global list of all the pairings: code for new reps and views can be
  dropped in without any changes to existing code (the old and the new
  can obviously share where so desired).  The rep mapping code can
  employ the session extensions, for example to examine user's
  preferences or to use experiment-specific services---such as
  introspection to get object properties when nothing better is
  available.

 @subsection archcomm Communication

  In our architecture communication takes place through three
  channels: session extensions, message services, and the object
  mapping methods mentioned above.  The first has already been
  covered.  The message services allow browsers to share messages such
  as ``I selected this'': all observers of a service can maintain
  together a coherent state and to answer queries from each other
  while still knowing next to nothing about the message sender.  The
  final channel is the object--rep--model operations, one of which was
  already mentioned: the creation of a rep.  Another operation commits
  a changed rep back to the object, yet another lazily expands an
  object rep (for example to read data only when it is requested).
  The operations can be extended, both to handle new argument
  combinations and with new methods with arbitrary parameter types
  (#IgBrowserMethods).

 @subsection archold Old Stuff (SKIP THIS!)

  This library provides the infrastructure architecture for <i>object
  browsing:</i> that some "representable" application objects can be
  "visualised" in a number of different ways.

  A strategic architectural choice is that the ability to visualise an
  object must have minimal impact on the object's design.  Thus, an
  object needs to inherit from a superclass provided by this package.
  The sole purpose of the superclass is to guarantee that the derived
  objects have run-time type information.  The infrastructure makes
  use of the run-time type information to operate on the application
  objects---it request no services from the objects themselves.  The
  necessary run-time type information is provided by the client
  packages, compiled it into the application or its shared libraries.
  The information is usually outside the classes so as not to disturb
  their design unnecessarily.

  "Visualising" should be understood liberally.  It can vary from
  fancy form like 3D visualisation to a mere text dump of an object's
  attributes, and more.  It can include ways of interacting with the
  objects.  For example, one view of an object can enable queries on
  the object: the "visualisation" of the object would be the interface
  that allows one to enter and execute a query.

  Each distinct way of looking at an object is called @em browsing.
  Every application class can be associated with any number of @em
  browsers that know how to visualise objects of that and derived
  types.  Browsers are usually designed to be independent from each
  other; the infrastructure provides a protocol for browsers to
  communicate and collaborate amongst themselves.  The protocol can be
  used to build a <i>composite browser</i> that appears as a neat
  integrated user interface, when in reality it is only a collection
  of independent browsers that know little or nothing about each
  other.

  Typically applications do not use any of the browsers directly, but
  will use this infrastructure to create and to talk to them.  Doing
  it this way has certain advantages.  Firstly, there is no direct
  coupling between the application and the browser, the application
  knows (and links against) only the abstract interfaces in this
  package.  Another advantage is that the infrastructure can load and
  unload the browsers and all the related resources on demand.

  What follows now is an overview of key concepts, a brief
  intruduction to how to use the services of this package from an
  application, and finally a number of sections on how to design
  browsers.  This latter part walks through the steps of designing a
  browser, implementing it, and making it talk to other browsers.
  This is followed by a description of how the dynamic loading of
  browser plug-in libraries works.


 @subsection archoldapps Old: Application drivers, extensions and services

  FIXME

 @subsection archoldgui Old: Sites, Browsers, Models and Representations

  The key concepts for this infrastructure relate to browser sites,
  browsers, and their intercommunication, as well as to representing
  application objects to the browsers.

  Browsers are the ones that "visualise" application objects.  They do
  not use the application objects directly, but instead
  representations of those objects.  The representations make up
  browser models: the model might be the representation of just one
  object, or it might include the representations of several objects.

  Browser sites "host" browsers, and can include places such as
  windows, subprocess pipes, or files to output to.  It can be assumed
  that there will be major categories of browsers and that browsers
  from the different categories may not co-exist seamlessly.  For
  example, sites built to use a specific windowing system library such
  as Java, GTK or Qt may not be able to host browsers that were built
  for another library.  The sites can, however, talk to each other
  using a protocol defined by this library (see the description of
  browser buses below), allowing some degree of co-operation, and a
  high degree of integration for browsers and sites that are built
  with compatible technologies.  This is further explained in the
  section on browser and site configuration.

  FIXME: Session!  (has the bus...)

  Browsers and sites can communicate to each other through browser
  buses, which are essentially broadcast message repeaters.  An object
  registers itself with a bus and defines message handlers for the
  message types it is interested in.  Messages it is not intrested in
  (or does not understand) it will never see.  To communicate to other
  participants, an object on the bus broadcasts a message of some
  particular type.  For example, a browser can notify other browsers
  that it has selected an object by broadcasting on the bus a
  #IgSelectMessage.  Note that the bus can be used for more than just
  notifications: messages can be sent to query information, to invoke
  actions on objects or to instantiate new browsers (sites, not
  browsers, would typically respond to this last type of message).  In
  a way, the buses should be seen as an enabling technogogy, enabling
  otherwise independent pieces of software to talk to each other.

  Let us now turn back to representing application objects in
  browsers.  A browser uses one or more models from which it creates
  its "views." The model contains or refers to the representations
  (#IgRep) of individual application objects (#IgRepresentable).  On
  the other hand, all the representations of an application object are
  chained into a representation set (a #IgRepSet), allowing the
  navigation from the representation of an object in one model to that
  in another model.

  It is largely the chaining of representations that allows browsers
  to communicate without having to know about each other.  The idea is
  that we form a chain of the representations of an object in all
  models (and therefore browsers) so that when the #IgRepresentable
  has to be shown in another browser, we can immediately tell what
  context the browser has to switch to to make the object visible.
  Or, if the object does not yet have a representation in that model,
  one can be created at that point.

  The chaining of representations is done using small context objects
  (#IgRepContext) that act as multi-way associations.  For lightweight
  cases the context and the representation may actually be the same
  object.  In any case, the representation sets for different
  application objects can often be different: since the
  representations are created on demand, the contents of the set
  depend on what browsers the object has been visualised in.

  (FIXME: browsers communicate by using the context for the object;
  other browsers navigate from the context to the reps they use, or
  create/augment them if they do not exist or are not complete, or are
  only future promises)

  The representation set needs to be able to deal with several
  constraints:

    - Browsers without a model in this application.  In this case the
      model is usually in some external application.  Whether a
      context is stored for the model depends on the communication
      channel to the browser.  If the channel is a one-way stream of
      data that has lost its connection to the application objects
      (e.g., converting objects to pure spatial data), no context is
      usually recorded.  If there is a way to label objects or to
      communicate to the browser---even unidirectionally---stub
      context objects may be created for future reference.

    - Browsers that have a streaming model where the model is
      reclaimed immediately after creation.  In fact, the "creation"
      of the model may be equivalent of writing it out directly
      without ever creating any #IgRep objects.

    - Browser models that are constructed from several different
      representations, or browsers that use several models.

    - Composite browsers where browsers may show different views of
      the same application objects, or may show different objects.
      Especially in the latter it may occur that an object A has
      representation X, the selection of which in one browser causes
      another browser to request representation Y.  Y should then be
      created and added to the set of representations for object A.
      Note that neither the browser for X nor the one for Y need to be
      using lazy models for this scenario to occur.

    - Browser models that are constructed greedily or lazily.  A
      greedily constructed model is constructed in one shot; this is
      significant when several #IgRepresentable objects participate
      in the model.  A lazy model is able to accomodate incremental
      construction.  Usually a lazy model starts out with the creation
      of representations for only a few objects with "promises" to
      expand on other objects in the future.  When the browser
      determines it wants to show the promised representations, it
      asks them to be completed.  The completion may create more
      future promises.

      Streaming models are usually greedy.  Lazy models are often
      connected to garbage collection, as the reason for the model to
      be lazy is often that the model would otherwise grow too large,
      or would take too much time to construct up front.  An
      interesting variant of lazy models are ones that are essentially
      statistical in their nature: they can accept an arbitrary object
      at an arbitrary time.  In this respect they are like streaming
      models, and can in fact be true one-way streams if the objects
      lose their identity.

    - Representations and models created within the application may
      require garbage collection.  Assume for instance that a user
      visualises several physics events from the database.  Once the
      user has moved from one event to another and the previous one is
      no longer shown, the model for the previous event could be
      destroyed.  However, if the user is going to switch back and
      forth between the events (for example to compare them), it does
      not make sense to destroy the unshown one immediately.  This is
      even more obvious in other scenarios where many browsers
      co-operate to form one user interface: each browser may have its
      own model, and it is difficult to predict how users will
      interact with them.  They might be switching back and forth
      among the objects, or may look at an object only once.

      To support this efficiently, there must be some form of garbage
      collection: representations that are currently being visualised
      must not be destroyed, but the ones that are not being
      visualised currently nor have been visible recently should be
      destroyed---at least if memory starts running short.

  Let us use a couple of examples to illustrate some of these
  concepts.  Imagine a browser whose purpose is to produce a XML file
  on the application objects.  In this case, the browser site would be
  the text file into which the XML would be written to.  The #IgRep of
  an application object would be a XML element (a tag): the name of
  the element, the attributes and their values, and the content of the
  element.  The XML model would naturally include reps of several
  objects.  For example, were one to browse a composite object such as
  an event, the resulting browser model would include not only the
  #IgRep for the container (the event), but also for all the objects
  it contains (the raw data, reconstruction results, and so forth).

  Another example would be a browser that links the object to its
  documentation.  In this case, the browser would be something that
  would show, for example, a button in a toolbar: the toolbar is the
  site and the button the browser.  Clicking the button would cause a
  web browser to go to a page documenting that object type, and
  perphaps allow the user to browse the design documents, code, change
  history, and recent bug fixes relating to that class.  It could even
  be that as the object is selected in another browser, the web page
  automatically updates to the documentation.  The #IgRep would simply
  be the URL: a text string.  The browser model would be a pointer to
  the "current" representation.

  FIXME: more examples?


 @subsection archoldbrowseobj Old: Browsing Objects

  FIXME: explained in #IgBrowser


 @subsection archoldbrowseintf Old: Browser Interface

  FIXME: explained in #IgBrowser


 @subsection archoldbrowseimpl Old: Browser Implementation

  FIXME: explained in #IgBrowser


 @subsection archoldbrowsecomm Old: Inter-Browser Communication

  FIXME: explained above?


 @subsection archoldbrowseconf Old: Browser and Site Configuration

  FIXME: use something like XUL from mozilla

  FIXME: different major (incompatible) categories of browsers, like
  streamers (xmlwriter), subprocesses, windowing systems (gtk, java,
  ...).  for java side, sites can be independent frames, embedded
  windows or flow objects (for compound document).  construction of
  compound browsers (e.g., hierarchical database browser +
  auto-switched property browser/histogram viewer/3d display + toolbar
  + status bar) needs to be clarified -- how to define one (using
  xml?), how to start one, how autoswitcing works.

  FIXME: compound document is an extension of sites: each flow object
  is a rectangular area, and we need to give clients access to usual
  windowing stuff in that area -- input (key, mouse, drag) and output
  (2d, 3d); see also OLE in-place editing + printing. will need a
  document editing environment, but this could perhaps use something
  like the free mozilla stuff, FOP, or shell it out into universities
  as student or research projects (sell it as writing a xsl flow
  object editor, that should be enough cutting and bleeding edge :-).


@section plugins Browser Plug-in Directories

  The browsers are installed in <i>plug-in directories</i>.  A file in
  such a directory is a XML file describing the properties of the
  plug-in (see #IgPlugin).  Each plug-in library must conform to the
  interface described in the next section.  The infrastructure loads
  the libraries on application demand.

  #IgPluginDatabase keeps track of available plug-in directories and
  libraries and what features are provided where.  The database can be
  queried for this information (FIXME: we've removed the capability to
  find out which browsers can show what objects!).  These descriptors
  also provide a link to the factories to create instances the various
  objects types.

  The information in the database is collected as follows.  The
  plug-in database reads the XML descriptions in the plug-in
  directories to find out which shared libraries to load.  It then
  loads each shared library in turn, queries for the features it
  provides, and unloads the library.  As it goes, it caches the
  information in a file in the plug-in directory.  The database uses
  the cache instead of querying the libraries if it detects the cache
  is valid, bypassing the possibly expensive operation of loading and
  unloading shared libraries, thus speeding up application start-up.
  The cache keeps track of time stamps of individual plug-in
  definitions and updates only those that have changed.

  Note that the plug-in directories do not contain any shared
  libraries, only XML fragments that tell the name of the library to
  load.  It is assumed that dynamic loader will just find the
  libraries, for instance because the @c LD_LIBRARY_PATH environment
  variable has been set to point to the directories where the
  libraries are installed in.

  The cache updates are fully automatic and require but one action
  from the part of the developers: if browser libraries are installed
  into a directory that is subsequently made read-only, the last stage
  of plug-in installation should run a program that causes the cache
  to be recreated while the directory is still writable.  Otherwise
  the browser architecture will be unable to create the cache later on
  and will be forced to load the plug-ins to collect the information.


@section pluginintf Browser Plug-in Interface

  Each browser plug-in library must define a class inheriting from
  #IgPluginDef and use the macro #DEFINE_IGUANA_PLUGIN to define the
  magic symbols needed by the infrastructure to gain access to that
  definition.  The class has three methods of interest: @c attach,
  @c detach and @c query.

  @c query is used to interrogate the plug-in about the features it
  provides.  When called, the library should invoke the @c declare
  methods in the base class with information of the features the
  library implements.  It should not attempt to instantiate any
  browsers or perform any other additional tasks.

  @c attach will be invoked when the plug-in is loaded with the
  intention of using the features it provides.  At this point the
  library should install factories for its object types by calling the
  |install| methods in the base class.  It should do so for every
  feature it declared in the @c query phase.  If @c query registered
  capabilities, code in @c attach should have the side effects to
  fulfill the capability promises.  For example, the modification of
  the multi-method tables for the browsing infrastructure is
  implemented in terms of capabilities: the "fulfillment" of the
  capability happens automatically when the dynamic loader executes
  the global constructors in the shared library.  If the library has
  some less conventional design, the @c attach method can do real
  work.

  @c detach will be invoked when the system is about to unload the
  plug-in library; it is the opposite of @c attach and should undo
  whatever @c attach did.  Typically there is no need to override this
  from the base class.  @c detach should not free the factories
  created in @c attach ---that will happen automatically.  Typical
  plug-ins will withdraw their multi-method extensions and run-time
  type information tables automatically as the dynamic loader runs the
  global destructors of the library upon unload, so no action is
  usually required from the library designer.  It is guaranteed that
  @c detach is called only if @c attach returned successfully.


@section except Exceptions

  FIXME

@section gloss Glossary

  <dl>
    <dt>#IgDriver, driver, application driver</dt>
      <dd>FIXME</dd>

    <dt>#IgExtension, extension, application extension</dt>
      <dd>FIXME</dd>

    <dt>Service</dt>
      <dd>FIXME</dd>

    <dt>#IgSession, session</dt>
      <dd>FIXME</dd>

    <dt>#IgBrowser, browser</dt>
      <dd>A way of visualising objects, navigating among them, and
      possibly editing them.  "Visualising" should be interpreted very
      liberally; it could be as simple as writing out some text, or as
      complicated as displaying and interacting with objects in a 3D
      environment.</dd>

    <dt>#IgRepresentable, representable</dt>
      <dd>An application object that can be visualised in
      browsers.</dd>

    <dt>#IgRep, representation</dt>
      <dd>The data or objects derived from a #IgRepresentable so that
      it can be visualised in a browser.  In the model-view-controller
      paradigm this is a view of the #IgRepresentable, but also
      usually the model (or part thereof) of a browser.</dd>

    <dt>#IgModel, browser model, model</dt>
      <dd>A data structure of #IgRep objects that a browser or
      browsers use in their visualisation.  It is called a model in
      the model-view-controller sense: it is the data the browser is
      displaying (in some very rare cases it might also be the view).
      However, from the application objects' point of view, it is a
      view.  Note that a model may consist of representations for
      several objects.</dd>

    <dt>#IgSite, site, browsing site</dt>
      <dd>The output area of a browser.  This could be a file for a
      browser that dumps out objects, or a window for a browser that
      shows objects visually.</dd>
  </dl>

  @sa #IgPluginDatabase, #IgPlugin, #IgSession, #IgDriver,
      #IgExtension, #IgRepresentable, #IgSite, #IgBrowser, #IgModel,
      #IgRep.


@section todo To be explained

   - are we dealing correctly with multiple browsers for same model
     type (e.g., multiple histograms)?
   - how are incomplete models (reps) augmented?
   - how are future (promised) models (reps) created and completed?
   - how is a browser switched with another one in the same site?
   - how do we limit the number of windows popping up while not using
     auto-switched browser areas?  maybe use something like html frames
     with named (no new windows, switch the browser/model in the named
     site) and unnamed (new window for browser/model) sites?
   - who does garbage collection (idea: "strong" and "weak" references
     to models/reps; weak ones age, and if enough is allocated and age
     is too old, reap them --> call finalise and destruct.)
   - how do we rip browsers apart (view, controls); see OLE in-place
     editing docs in MsDev (MSDN Knowledge Database?) -- need to enum
     controls (menu, toolbar, statusbar, floating, ...) and allow edit
     or window creation in place
   - how do we find out which sites a browser can use? (Known?)
   - how do we construct applications from browsers? browser containers?
     must be easy so that relatively little knowledge is required and
     even a normal user could do it relatively easily.  ==> XML (XUL?)
     (Or BML from IBM?)
   - can we do compound documents?
   - can we find a flow object formatter?  RenderX?  FOP?
   - how do we deal with multi-language issues?

@section tododone To be explained (should be ok)
   - how is a browser created/started/loaded?
   - how is a browser's model (reps) created?  */

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

/** Abstract object browser base class.

  #IgBrowser defines the abstract base class for object browsers.  The
  object browser interface extends beyond this base class declaration:
  #IgBrowser is a simple polymorphic class with little functionality
  in it, most features are implemented with multi-methods.

@subsection iface Interface

  It is assumed that the browser has one more #IgModel objects that
  store representations (#IgRep) for application-domain objects
  (#IgRepresentable).  It is up the model how many objects it can
  simultaneously represent.  It is from these models the browser is
  expected to "render" the "view" it shows to users---not from the
  application-domain objects.  The "rendered view" might be a simple
  dump of object contents to the terminal, or a more complex
  representation of the data as an interactive 3D visualisation.

  The #IgBrowser always participates in an session (#IgSession); it
  gets the reference to the session in the constructor via the factory
  (see #IgBrowserFactory).  The session has a bus (#IgBus) that the
  browser should use to communicate with other browsers; this avoids
  the need to build strong coupling among the browsers.  The derived
  class should communicate on the bus interesting events such as
  selection of an application-domain object.  Other browsers then have
  the opportunity to react to this, for instance by generating
  representations for the selected object and then rendering them.

  The site in which the browser is to exist is given as a constructor
  argument (see #IgBrowserFactory).

  The only feature #IgBrowser provide directly is a way to initiate
  the browsing of a #IgRepresentable.  This is done by calling the
  #browse method.

@subsection impl Implementation

  Deriving class should implement the #browse method as follows.  The
  method should find or create the object's representation in all the
  models the browser uses; the easiest way to do so is to simply
  invoke IgRepSet::lookup().  The browser may then make the rep the
  current selection by broadcasting a #IgSelectMessage through the
  #IgSelectionService in the session the browser belongs to.  The
  message will be delivered to the browser itself, at which point the
  browser should make the object visible and the current selection, or
  carry out some an action similar to these as appropriate to the
  function of the browser.  (FIXME: this doesn't make sense for all
  multi-object browsers -- do we really want the select?)

  It is up to the browser to decide how it manages its models.  This
  interface does not mandate any particular behaviour or design, only
  that the browser conforms to the conceptual abstract design.  It is,
  however, recommended that browsers attempt to share models where
  possible.  */

class IGUANA_FRAMEWORK_API IgBrowser
{
public:
    virtual ~IgBrowser (void);

    /** Begin browsing the object.

      The implementation must represent @a object in all its models.
      @a object (its representation, that is) may then be made the
      current selection of this and all other browsers in the same
      #IgSession as this browser.  This last step should be
      implemented by broadcasting a #IgSelectMessage on the session
      #IgSelectionService; the browser itself can simply respond to
      the message as it is delivered to it.

    @note The browser has to create a model and a rep of the object;
          even simple dummy ones will do.  These can be discarded once
          the other browsers have been notified of the selection
          change.

    @sa See the class documentation for a more complete description.

    @param object The object to visualise.  Even though the object is
                  not @c const, the browser should not assume it can
                  change it.  All non-editor browsers should treat the
                  object purely as @c const.  Editors should treat the
                  object as @c const as long as possible, preferably
                  until the user explicitly requests the object to be
                  changed.  Thus, editors should make changes only to
                  the representation until it is clear that the user
                  is committed to making the changes to the object,
                  too.  */

    virtual void	browse (IgRepresentable *object) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_BROWSER_H
